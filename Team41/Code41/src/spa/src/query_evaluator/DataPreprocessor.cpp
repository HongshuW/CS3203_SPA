//
// Created by Nafour on 5/9/22.
//

#include "DataPreprocessor.h"

#include <string>
#include <utility>

#include "QEUtils.h"
#include "TableCombiner.h"
#include "constants/ClauseVisitorConstants.h"
#include "constants/QueryEvaluatotConstants.h"
#include "query_builder/clauses/pattern_clauses/AssignPatternClause.h"
#include "query_builder/clauses/pattern_clauses/IfPatternClause.h"
#include "query_builder/clauses/pattern_clauses/WhilePatternClause.h"
#include "query_builder/clauses/select_clauses/SelectClause.h"
#include "query_builder/clauses/such_that_clauses/AffectsClause.h"
#include "query_builder/clauses/such_that_clauses/AffectsTClause.h"
#include "query_builder/clauses/such_that_clauses/CallsClause.h"
#include "query_builder/clauses/such_that_clauses/CallsTClause.h"
#include "query_builder/clauses/such_that_clauses/FollowsClause.h"
#include "query_builder/clauses/such_that_clauses/FollowsTClause.h"
#include "query_builder/clauses/such_that_clauses/ModifiesPClause.h"
#include "query_builder/clauses/such_that_clauses/ModifiesSClause.h"
#include "query_builder/clauses/such_that_clauses/NextClause.h"
#include "query_builder/clauses/such_that_clauses/NextTClause.h"
#include "query_builder/clauses/such_that_clauses/ParentClause.h"
#include "query_builder/clauses/such_that_clauses/ParentTClause.h"
#include "query_builder/clauses/such_that_clauses/UsesPClause.h"
#include "query_builder/clauses/such_that_clauses/UsesSClause.h"
#include "query_builder/commons/DesignEntity.h"
#include "query_builder/commons/WithRef.h"

using namespace std;
using namespace QB;
namespace QE {

auto& entitiesToFilter = QueryEvaluatotConstants::entitiesToFilter;
auto& attrsToFilter = QueryEvaluatotConstants::attrsToFilter;

shared_ptr<Table> DataPreprocessor::getAllByDesignEntity(
    DesignEntity designEntity) {
  const int FIRST_COL_IDX = 0;
  const vector<DesignEntity> AVAIL_ENTITIES = {
      DesignEntity::STMT, DesignEntity::CONSTANT, DesignEntity::VARIABLE,
      DesignEntity::PROCEDURE};
  const int SECOND_COL = 1;
  if (count(AVAIL_ENTITIES.begin(), AVAIL_ENTITIES.end(), designEntity)) {
    auto resultTable = make_shared<Table>(
        this->dataRetriever->getTableByDesignEntity(designEntity));
    resultTable->dropColFromThis(SECOND_COL);
    resultTable->removeDupRow();
    return resultTable;
  }

  // assign, call, print, if, while , read
  auto resultTable = make_shared<Table>(
      this->dataRetriever->getTableByDesignEntity(QB::DesignEntity::STMT));
  this->filerTableByDesignEntity(resultTable, FIRST_COL_IDX, designEntity);
  resultTable->dropColFromThis(SECOND_COL);
  resultTable->removeDupRow();
  return resultTable;
}

DesignEntity DataPreprocessor::getDesignEntityOfSyn(const Synonym& synonym) {
  for (const auto& d : *declarations) {
    if (synonym.synonym == d.getSynonym().synonym) {
      return d.getDesignEntity();
    }
  }
  return static_cast<DesignEntity>(NULL);
}

vector<int> DataPreprocessor::getStmtNumsByDesignEntity(
    DesignEntity designEntity) {
  auto table = this->getAllByDesignEntity(designEntity);
  int STMT_NO_COL_IDX = 0;
  auto stmtNums = table->getColumnByName(table->getHeader()[STMT_NO_COL_IDX]);
  vector<int> ans;
  for (const auto& string : stmtNums) {
    ans.push_back(stoi(string));
  }
  return ans;
}

vector<string> DataPreprocessor::getEntityNames(DesignEntity designEntity) {
  vector<DesignEntity> AVAIL_ENTITIES = {
      DesignEntity::CONSTANT, DesignEntity::VARIABLE, DesignEntity::PROCEDURE};
  if (count(AVAIL_ENTITIES.begin(), AVAIL_ENTITIES.end(), designEntity)) {
    int ENTITY_NAME_COL_IDX = 0;
    auto table = this->dataRetriever->getTableByDesignEntity(designEntity);
    return table.getColumnByName(table.getHeader()[ENTITY_NAME_COL_IDX]);
  }
  return {};
}

shared_ptr<Table> DataPreprocessor::getTableByWith(
    const shared_ptr<WithClause>& withClause) {
  bool hasSynonym = withClause->lhsType() == WithRefType::ATTR_REF ||
                    withClause->rhsType() == WithRefType::ATTR_REF;
  // if the with clause does not have synonym, don't have to care what's inside
  // since I have checked this clause evaluates to true
  if (!hasSynonym) return QEUtils::getScalarResponse(true);

  int intRef;
  string identRef;
  bool isIntType = true;

  vector<WithRef> withRefs = {withClause->lhs, withClause->rhs};
  auto resultTable = make_shared<Table>();
  vector<int> comparedCols = {};
  int colOffSet = 0;

  for (auto withRef : withRefs) {
    WithRefType withRefType = WithClause::getWithRefType((int)withRef.index());
    if (withRefType == QB::WithRefType::INTEGER)
      intRef = get<WithClause::WITH_REF_INT_IDX>(withRef);
    if (withRefType == QB::WithRefType::IDENT) {
      isIntType = false;
      identRef = get<WithClause::WITH_REF_IDENT_IDX>(withRef).identStr;
    }
    if (withRefType != WithRefType::ATTR_REF) continue;

    AttrRef attrRef = get<WithClause::WITH_REF_ATTR_REF_IDX>(withRef);
    DesignEntity designEntity = getDesignEntityOfSyn(attrRef.synonym);

    bool processingNeeded = std::count(entitiesToFilter.begin(),
                                       entitiesToFilter.end(), designEntity) &&
                            std::count(attrsToFilter.begin(),
                                       attrsToFilter.end(), attrRef.attrName);

    const int TABLE_HEADER_SIZE = 2;
    if (!processingNeeded) {
      auto table = getAllByDesignEntity(designEntity);
      if (table->isBodyEmpty()) return make_shared<Table>();
      const int FIRST_COL_IDX = 0;
      const int DROP_COL_FROM = 1;

      table->dropColFromThis(DROP_COL_FROM);
      table->removeDupRow();
      // get two identical cols with different names
      table->dupCol(FIRST_COL_IDX);
      table->renameHeader({attrRef.synonym.synonym, attrRef.toString()});
      resultTable = TableCombiner().crossProduct(resultTable, table);

      // since two cols are identical, compare either is alright
      comparedCols.push_back(colOffSet);
      colOffSet += (int)TABLE_HEADER_SIZE;
    } else {
      const int ATTR_NAME_COL_INX = 1;

      auto intermediateTable = make_shared<Table>();  // get results
      if (designEntity == QB::DesignEntity::CALL)
        intermediateTable =
            make_shared<Table>(dataRetriever->getCallsProcedureNames());
      if (designEntity == QB::DesignEntity::READ)
        intermediateTable =
            make_shared<Table>(dataRetriever->getReadVariableNames());
      if (designEntity == QB::DesignEntity::PRINT)
        intermediateTable =
            make_shared<Table>(dataRetriever->getPrintVariableNames());

      intermediateTable->renameHeader(
          {attrRef.synonym.synonym, attrRef.toString()});
      resultTable =
          TableCombiner().crossProduct(resultTable, intermediateTable);

      // compare the second col which is the attribute value
      comparedCols.push_back(colOffSet + ATTR_NAME_COL_INX);
      colOffSet += (int)TABLE_HEADER_SIZE;
    }
  }
  if (withClause->lhsType() == WithRefType::ATTR_REF &&
      withClause->rhsType() == WithRefType::ATTR_REF) {
    filterTableByColValueEquality(resultTable, comparedCols);
  } else {
    const int ATTR_NAME_COL_IDX = 1;
    filerTableByColumnIdx(resultTable, ATTR_NAME_COL_IDX,
                          isIntType ? to_string(intRef) : identRef);
  }

  return resultTable;
}

void DataPreprocessor::filterTableByColValueEquality(
    const shared_ptr<Table>& table, const vector<int>& comparedCols) {
  Table filteredTable = Table();

  filteredTable.renameHeader(table->getHeader());

  vector<vector<string>>* rowsPtr = table->getRowsPointer();
  auto rowItr = rowsPtr->begin();
  while (rowItr != rowsPtr->end()) {
    unordered_set<string> set;
    for (int colIdx : comparedCols) {
      set.insert(rowItr->at(colIdx));
    }
    if (set.size() <= 1) {
      ++rowItr;
      continue;
    }
    *rowItr = (*rowsPtr)[table->getNumberOfRows() - 1];
    rowsPtr->erase(rowsPtr->end() - 1);
  }
}

void DataPreprocessor::filterSingleClauseResultTable(
    Ref ref1, Ref ref2, const shared_ptr<Table>& table) {
  // RefType can be synonym, integer, underscore or string
  RefType ref1Type = getRefType(ref1);
  RefType ref2Type = getRefType(ref2);
  string col1Name = ref1Type == QB::RefType::SYNONYM
                        ? get<Synonym>(ref1).synonym
                        : QEUtils::getColNameByRefType(ref1Type);
  string col2Name = ref2Type == QB::RefType::SYNONYM
                        ? get<Synonym>(ref2).synonym
                        : QEUtils::getColNameByRefType(ref2Type);

  vector<string> newHeaders = vector<string>{col1Name, col2Name};
  table->renameHeader(newHeaders);

  const int FIRST_COL_IDX = 0;
  const int SECOND_COL_IDX = 1;

  switch (ref1Type) {
    case RefType::SYNONYM: {
      Synonym syn1 = get<Synonym>(ref1);
      this->filerTableByDesignEntity(table, FIRST_COL_IDX,
                                     this->getDesignEntityOfSyn(syn1));
      break;
    }
    case RefType::UNDERSCORE:
      break;
    case RefType::INTEGER: {
      int int1 = get<int>(ref1);
      this->filerTableByColumnIdx(table, FIRST_COL_IDX, to_string(int1));
      break;
    }
    case RefType::IDENT:
      Ident ident1 = get<Ident>(ref1);

      this->filerTableByColumnIdx(table, FIRST_COL_IDX, ident1.identStr);
      break;
  }
  switch (ref2Type) {
    case RefType::SYNONYM: {
      Synonym syn2 = get<Synonym>(ref2);
      this->filerTableByDesignEntity(table, SECOND_COL_IDX,
                                     this->getDesignEntityOfSyn(syn2));

      // synonyms are the same: equality check between the two cols
      if (ref1Type != QB::RefType::SYNONYM) break;
      Synonym syn1 = get<Synonym>(ref1);
      if (!(syn1 == syn2)) break;

      this->filterTableByColValueEquality(table,
                                          {FIRST_COL_IDX, SECOND_COL_IDX});
      break;
    }
    case RefType::UNDERSCORE:
      break;
    case RefType::INTEGER: {
      int int2 = get<int>(ref2);
      this->filerTableByColumnIdx(table, SECOND_COL_IDX, to_string(int2));

      if (ref1Type != QB::RefType::INTEGER) break;
      int int1 = get<int>(ref1);
      if (int1 != int2) break;
      this->filterTableByColValueEquality(table,
                                          {FIRST_COL_IDX, SECOND_COL_IDX});
      break;
    }
    case RefType::IDENT:
      Ident ident2 = get<Ident>(ref2);
      this->filerTableByColumnIdx(table, SECOND_COL_IDX, ident2.identStr);

      if (ref1Type != QB::RefType::IDENT) break;
      Ident ident1 = get<Ident>(ref1);
      if (!(ident1 == ident2)) break;
      this->filterTableByColValueEquality(table,
                                          {FIRST_COL_IDX, SECOND_COL_IDX});
      break;
  }
  bool isDropped = dropUnusedColumns(table);
  if (isDropped) table->removeDupRow();
}

DataPreprocessor::DataPreprocessor(shared_ptr<DataRetriever> dataRetriever,
                                   Declarations declarations)
    : dataRetriever(std::move(dataRetriever)),
      declarations(std::move(declarations)) {}

shared_ptr<Table> DataPreprocessor::getTableByAssignPattern(
    const shared_ptr<QB::AssignPatternClause>& assignPatternClause) {
  auto table = make_shared<Table>(
      dataRetriever->getAssignPatternTable(assignPatternClause->arg3));
  filterSingleClauseResultTable(assignPatternClause->arg1,
                                assignPatternClause->arg2, table);
  return table;
}

shared_ptr<Table> DataPreprocessor::getIfPatternTable(
    const shared_ptr<QB::IfPatternClause>& ifPatternClause) {
  auto table = make_shared<Table>(dataRetriever->getIfPatternTable());
  filterSingleClauseResultTable(ifPatternClause->arg1, ifPatternClause->arg2,
                                table);
  return table;
}

shared_ptr<Table> DataPreprocessor::getWhilePatternTable(
    const shared_ptr<WhilePatternClause>& whilePatternClause) {
  auto table = make_shared<Table>(dataRetriever->getWhilePatternTable());
  filterSingleClauseResultTable(whilePatternClause->arg1,
                                whilePatternClause->arg2, table);
  return table;
}

shared_ptr<Table> DataPreprocessor::getTableByFollows(
    const shared_ptr<FollowsClause>& followsClause) {
  auto table = make_shared<Table>(dataRetriever->getFollowsTable());
  filterSingleClauseResultTable(followsClause->arg1, followsClause->arg2,
                                table);
  return table;
}

shared_ptr<Table> DataPreprocessor::getTableByFollowsT(
    const shared_ptr<FollowsTClause>& followsTClause) {
  auto table = make_shared<Table>(dataRetriever->getFollowsTTable());
  filterSingleClauseResultTable(followsTClause->arg1, followsTClause->arg2,
                                table);
  return table;
}

shared_ptr<Table> DataPreprocessor::getTableByAffects(
    const shared_ptr<AffectsClause>& affectsClause) {
  const vector<DesignEntity> validTypes = {DesignEntity::STMT,
                                           DesignEntity::ASSIGN};
  const shared_ptr<Table> EMPTY_RESULT = make_shared<Table>();

  Ref ref1 = affectsClause->arg1;
  Ref ref2 = affectsClause->arg2;
  RefType ref1Type = getRefType(ref1);
  RefType ref2Type = getRefType(ref2);

  string col1Name = ref1Type == QB::RefType::SYNONYM
                        ? get<Synonym>(ref1).synonym
                        : QEUtils::getColNameByRefType(ref1Type);
  string col2Name = ref2Type == QB::RefType::SYNONYM
                        ? get<Synonym>(ref2).synonym
                        : QEUtils::getColNameByRefType(ref2Type);

  const int FIRST_COL_IDX = 0;
  const int SECOND_COL_IDX = 1;

  if (ref1Type == QB::RefType::INTEGER && ref2Type == QB::RefType::INTEGER) {
    auto boolTable =
        dataRetriever->getAffectsResult(get<int>(ref1), get<int>(ref2));
    return boolTable.isEqual(ClauseVisitorConstants::TRUE_TABLE)
               ? QEUtils::getScalarResponse(true)
               : EMPTY_RESULT;
  }
  if (ref1Type == QB::RefType::INTEGER) {
    auto resultTable = make_shared<Table>(
        dataRetriever->getAffectedStatements(get<int>(ref1)));
    resultTable->dropColFromThis(FIRST_COL_IDX);
    resultTable->removeDupRow();
    // check if second ref is not a synonym and return result immediately
    if (ref2Type != QB::RefType::SYNONYM)
      return !resultTable->isBodyEmpty() ? QEUtils::getScalarResponse(true)
                                         : EMPTY_RESULT;
    bool isValidType = std::count(validTypes.begin(), validTypes.end(),
                                  getDesignEntityOfSyn(get<Synonym>(ref2)));
    if (!isValidType) return EMPTY_RESULT;
    // the second ref must be a synonym
    resultTable->renameHeader({col2Name});
    return resultTable;
  }
  if (ref2Type == QB::RefType::INTEGER) {
    auto resultTable = make_shared<Table>(
        dataRetriever->getAffectingStatements(get<int>(ref2)));
    resultTable->dropColFromThis(SECOND_COL_IDX);
    resultTable->removeDupRow();
    if (ref1Type != QB::RefType::SYNONYM)
      return !resultTable->isBodyEmpty() ? QEUtils::getScalarResponse(true)
                                         : EMPTY_RESULT;
    bool isValidType = std::count(validTypes.begin(), validTypes.end(),
                                  getDesignEntityOfSyn(get<Synonym>(ref1)));
    if (!isValidType) return EMPTY_RESULT;
    resultTable->renameHeader({col1Name});
    return resultTable;
  }
  bool isRef1Valid = ref1Type == QB::RefType::UNDERSCORE ||
                     (ref1Type == QB::RefType::SYNONYM &&
                      std::count(validTypes.begin(), validTypes.end(),
                                 getDesignEntityOfSyn(get<Synonym>(ref1))));
  bool isRef2Valid = ref2Type == QB::RefType::UNDERSCORE ||
                     (ref2Type == QB::RefType::SYNONYM &&
                      std::count(validTypes.begin(), validTypes.end(),
                                 getDesignEntityOfSyn(get<Synonym>(ref2))));
  if (!isRef1Valid || !isRef2Valid) return EMPTY_RESULT;
  auto resultTable = make_shared<Table>(dataRetriever->getAffectsTable());
  filterSingleClauseResultTable(ref1, ref2, resultTable);
  return resultTable;
}

shared_ptr<Table> DataPreprocessor::getTableByAffectsT(
    const shared_ptr<AffectsTClause>& affectsTClause) {
  const shared_ptr<Table> EMPTY_RESULT = make_shared<Table>();
  const vector<DesignEntity> validTypes = {DesignEntity::STMT,
                                           DesignEntity::ASSIGN};
  Ref ref1 = affectsTClause->arg1;
  Ref ref2 = affectsTClause->arg2;
  RefType ref1Type = getRefType(ref1);
  RefType ref2Type = getRefType(ref2);

  string col1Name = ref1Type == QB::RefType::SYNONYM
                        ? get<Synonym>(ref1).synonym
                        : QEUtils::getColNameByRefType(ref1Type);
  string col2Name = ref2Type == QB::RefType::SYNONYM
                        ? get<Synonym>(ref2).synonym
                        : QEUtils::getColNameByRefType(ref2Type);

  const int FIRST_COL_IDX = 0;
  const int SECOND_COL_IDX = 1;

  if (ref1Type == QB::RefType::INTEGER && ref2Type == QB::RefType::INTEGER) {
    auto boolTable =
        dataRetriever->getAffectsTResult(get<int>(ref1), get<int>(ref2));
    return boolTable.isEqual(ClauseVisitorConstants::TRUE_TABLE)
               ? QEUtils::getScalarResponse(true)
               : EMPTY_RESULT;
  }
  if (ref1Type == QB::RefType::INTEGER) {
    auto resultTable = make_shared<Table>(
        dataRetriever->getAffectedTStatements(get<int>(ref1)));
    resultTable->dropColFromThis(FIRST_COL_IDX);
    resultTable->removeDupRow();
    // check if second ref is not a synonym and return result immediately
    if (ref2Type != QB::RefType::SYNONYM)
      return !resultTable->isBodyEmpty() ? QEUtils::getScalarResponse(true)
                                         : EMPTY_RESULT;
    bool isValidType = std::count(validTypes.begin(), validTypes.end(),
                                  getDesignEntityOfSyn(get<Synonym>(ref2)));
    if (!isValidType) return EMPTY_RESULT;
    // the second ref must be a synonym
    resultTable->renameHeader({col2Name});
    return resultTable;
  }
  if (ref2Type == QB::RefType::INTEGER) {
    auto resultTable = make_shared<Table>(
        dataRetriever->getAffectingTStatements(get<int>(ref2)));
    resultTable->dropColFromThis(SECOND_COL_IDX);
    resultTable->removeDupRow();
    if (ref1Type != QB::RefType::SYNONYM)
      return !resultTable->isBodyEmpty() ? QEUtils::getScalarResponse(true)
                                         : EMPTY_RESULT;
    bool isValidType = std::count(validTypes.begin(), validTypes.end(),
                                  getDesignEntityOfSyn(get<Synonym>(ref1)));
    if (!isValidType) return EMPTY_RESULT;
    resultTable->renameHeader({col1Name});
    return resultTable;
  }
  bool isRef1Valid = ref1Type == QB::RefType::UNDERSCORE ||
                     (ref1Type == QB::RefType::SYNONYM &&
                      std::count(validTypes.begin(), validTypes.end(),
                                 getDesignEntityOfSyn(get<Synonym>(ref1))));
  bool isRef2Valid = ref2Type == QB::RefType::UNDERSCORE ||
                     (ref2Type == QB::RefType::SYNONYM &&
                      std::count(validTypes.begin(), validTypes.end(),
                                 getDesignEntityOfSyn(get<Synonym>(ref2))));
  if (!isRef1Valid || !isRef2Valid) return EMPTY_RESULT;

  auto resultTable = make_shared<Table>(dataRetriever->getAffectsTTable());
  filterSingleClauseResultTable(ref1, ref2, resultTable);
  return resultTable;
}

shared_ptr<Table> DataPreprocessor::getTableByCalls(
    const shared_ptr<CallsClause>& callsClause) {
  auto table = make_shared<Table>(dataRetriever->getCallsTable());
  filterSingleClauseResultTable(callsClause->arg1, callsClause->arg2, table);
  return table;
}

shared_ptr<Table> DataPreprocessor::getTableByCallsT(
    const shared_ptr<CallsTClause>& callsTClause) {
  auto table = make_shared<Table>(dataRetriever->getCallsTTable());
  filterSingleClauseResultTable(callsTClause->arg1, callsTClause->arg2, table);
  return table;
}

shared_ptr<Table> DataPreprocessor::getTableByModifiesP(
    const shared_ptr<ModifiesPClause>& modifiesPClause) {
  auto table = make_shared<Table>(dataRetriever->getModifiesPTable());
  filterSingleClauseResultTable(modifiesPClause->arg1, modifiesPClause->arg2,
                                table);
  return table;
}

shared_ptr<Table> DataPreprocessor::getTableByModifiesS(
    const shared_ptr<ModifiesSClause>& modifiesSClause) {
  auto table = make_shared<Table>(dataRetriever->getModifiesSTable());
  filterSingleClauseResultTable(modifiesSClause->arg1, modifiesSClause->arg2,
                                table);
  return table;
}

shared_ptr<Table> DataPreprocessor::getTableByNext(
    const shared_ptr<NextClause>& nextClause) {
  auto table = make_shared<Table>(dataRetriever->getNextTable());
  filterSingleClauseResultTable(nextClause->arg1, nextClause->arg2, table);
  return table;
}

shared_ptr<Table> DataPreprocessor::getTableByNextT(
    const shared_ptr<NextTClause>& nextTClause) {
  Ref ref1 = nextTClause->arg1;
  Ref ref2 = nextTClause->arg2;
  RefType ref1Type = getRefType(ref1);
  RefType ref2Type = getRefType(ref2);

  if (ref1Type == QB::RefType::INTEGER && ref2Type == QB::RefType::INTEGER) {
    auto boolTable =
        dataRetriever->getNextTResult(get<int>(ref1), get<int>(ref2));
    return boolTable.isEqual(ClauseVisitorConstants::TRUE_TABLE)
               ? QEUtils::getScalarResponse(true)
               : make_shared<Table>();
  }
  auto resultTable = make_shared<Table>();
  if (ref1Type == QB::RefType::INTEGER) {
    resultTable =
        make_shared<Table>(dataRetriever->getNextTStatements(get<int>(ref1)));
    // check if second ref is not a synonym and return result immediately
    if (ref2Type != QB::RefType::SYNONYM)
      return !resultTable->isBodyEmpty() ? QEUtils::getScalarResponse(true)
                                         : make_shared<Table>();
  } else if (ref2Type == QB::RefType::INTEGER) {
    resultTable = make_shared<Table>(
        dataRetriever->getPreviousTStatements(get<int>(ref2)));
    if (ref1Type != QB::RefType::SYNONYM)
      return !resultTable->isBodyEmpty() ? QEUtils::getScalarResponse(true)
                                         : make_shared<Table>();
  } else {
    resultTable = make_shared<Table>(dataRetriever->getNextTTable());
  }
  filterSingleClauseResultTable(ref1, ref2, resultTable);
  return resultTable;
}

shared_ptr<Table> DataPreprocessor::getTableByParent(
    const shared_ptr<ParentClause>& parentClause) {
  auto table = make_shared<Table>(dataRetriever->getParentTable());
  filterSingleClauseResultTable(parentClause->arg1, parentClause->arg2, table);
  return table;
}

shared_ptr<Table> DataPreprocessor::getTableByParentT(
    const shared_ptr<ParentTClause>& parentTClause) {
  auto table = make_shared<Table>(dataRetriever->getParentTTable());
  filterSingleClauseResultTable(parentTClause->arg1, parentTClause->arg2,
                                table);
  return table;
}

shared_ptr<Table> DataPreprocessor::getTableByUsesP(
    const shared_ptr<UsesPClause>& usesPClause) {
  auto table = make_shared<Table>(dataRetriever->getUsesPTable());
  filterSingleClauseResultTable(usesPClause->arg1, usesPClause->arg2, table);
  return table;
}

shared_ptr<Table> DataPreprocessor::getTableByUsesS(
    const shared_ptr<UsesSClause>& usesSClause) {
  auto table = make_shared<Table>(dataRetriever->getUsesSTable());
  filterSingleClauseResultTable(usesSClause->arg1, usesSClause->arg2, table);
  return table;
}

shared_ptr<Table> DataPreprocessor::getCallsProcedureTable() {
  return make_shared<Table>(dataRetriever->getCallsProcedureNames());
}

shared_ptr<Table> DataPreprocessor::getReadVariableTable() {
  return make_shared<Table>(dataRetriever->getReadVariableNames());
}

shared_ptr<Table> DataPreprocessor::getPrintVariableTable() {
  return make_shared<Table>(dataRetriever->getPrintVariableNames());
}

shared_ptr<Table> DataPreprocessor::getNoConditionSelectClauseResult(
    const shared_ptr<SelectClause>& selectClause) {
  TableCombiner tableCombiner = TableCombiner();
  auto resultTable = make_shared<Table>();
  for (auto elem : *selectClause->returnResults) {
    if (elem.index() == SelectClause::ELEM_SYN_IDX) {
      Synonym synonym = std::get<SelectClause::ELEM_SYN_IDX>(elem);
      DesignEntity designEntity = getDesignEntityOfSyn(synonym);

      auto intermediateTable = getAllByDesignEntity(designEntity);
      if (intermediateTable->isBodyEmpty()) {
        return make_shared<Table>();
      }
      intermediateTable->renameHeader({synonym.synonym});
      resultTable = tableCombiner.crossProduct(resultTable, intermediateTable);
    } else {
      // elem is a attrRef
      AttrRef attrRef = std::get<AttrRef>(elem);
      DesignEntity designEntity = getDesignEntityOfSyn(attrRef.synonym);
      auto intermediateTable = getAllByDesignEntity(designEntity);

      if (intermediateTable->isBodyEmpty()) {
        return make_shared<Table>();
      }

      bool processingNeeded =
          std::count(entitiesToFilter.begin(), entitiesToFilter.end(),
                     designEntity) &&
          std::count(attrsToFilter.begin(), attrsToFilter.end(),
                     attrRef.attrName);

      if (processingNeeded) {
        if (designEntity == QB::DesignEntity::CALL)
          intermediateTable = getCallsProcedureTable();
        if (designEntity == QB::DesignEntity::PRINT)
          intermediateTable = getPrintVariableTable();
        if (designEntity == QB::DesignEntity::READ)
          intermediateTable = getReadVariableTable();
        if (intermediateTable->isBodyEmpty()) return make_shared<Table>();
      } else {
        for (int i = 1; i < intermediateTable->getNumberOfColumns(); i++) {
          intermediateTable->dropColFromThis(i);
        }
        const int FIRST_COL_IDX = 0;
        intermediateTable->dupCol(FIRST_COL_IDX);
      }
      intermediateTable->renameHeader(
          {attrRef.synonym.synonym, attrRef.toString()});
      resultTable = tableCombiner.crossProduct(intermediateTable, resultTable);
    }
  }
  return resultTable;
}

bool DataPreprocessor::hasResult(const shared_ptr<WithClause>& withClause) {
  Value valLHS;
  Value valRHS;

  vector<WithRef> withRefs = {withClause->lhs, withClause->rhs};
  shared_ptr<vector<Value>> values = make_shared<vector<Value>>();
  values->push_back(valLHS);
  values->push_back(valRHS);

  // get values from each ref and store in val1 and val2
  getWithValues(withRefs, values);
  valLHS = values->at(0);
  valRHS = values->at(1);

  // compare two values, assumption made: two values are of the same lhsType,
  // e.g. either int or string
  switch (valLHS.index()) {
    case ClauseVisitorConstants::STR_VAL_IDX: {
      string lhsStr = get<ClauseVisitorConstants::STR_VAL_IDX>(valLHS);
      if (valRHS.index() == ClauseVisitorConstants::STR_VAL_IDX)
        return lhsStr == get<ClauseVisitorConstants::STR_VAL_IDX>(valRHS);

      if (valRHS.index() == ClauseVisitorConstants::VECTOR_STR_VAL_IDX) {
        vector<string> strs =
            get<ClauseVisitorConstants::VECTOR_STR_VAL_IDX>(valRHS);
        return count(strs.begin(), strs.end(), lhsStr);
      }
      break;
    }
    case ClauseVisitorConstants::INT_VAL_IDX: {
      int lhsInt = get<ClauseVisitorConstants::INT_VAL_IDX>(valLHS);
      if (valRHS.index() == ClauseVisitorConstants::INT_VAL_IDX)
        return lhsInt == get<ClauseVisitorConstants::INT_VAL_IDX>(valRHS);
      if (valRHS.index() == ClauseVisitorConstants::VECTOR_INT_VAL_IDX) {
        vector<int> ints =
            get<ClauseVisitorConstants::VECTOR_INT_VAL_IDX>(valRHS);
        return count(ints.begin(), ints.end(), lhsInt);
      }
      break;
    }
    case ClauseVisitorConstants::VECTOR_STR_VAL_IDX: {
      vector<string> lhsVecStr =
          get<ClauseVisitorConstants::VECTOR_STR_VAL_IDX>(valLHS);
      if (valRHS.index() == ClauseVisitorConstants::STR_VAL_IDX) {
        string rhsStr = get<ClauseVisitorConstants::STR_VAL_IDX>(valRHS);
        return count(lhsVecStr.begin(), lhsVecStr.end(), rhsStr);
      }
      if (valRHS.index() == ClauseVisitorConstants::VECTOR_STR_VAL_IDX) {
        vector<string> rhsVecStr =
            get<ClauseVisitorConstants::VECTOR_STR_VAL_IDX>(valRHS);
        return !intersection(lhsVecStr, rhsVecStr).empty();
      }
      break;
    }

    case ClauseVisitorConstants::VECTOR_INT_VAL_IDX: {
      vector<int> lhsVecInt =
          get<ClauseVisitorConstants::VECTOR_INT_VAL_IDX>(valLHS);
      if (valRHS.index() == ClauseVisitorConstants::INT_VAL_IDX) {
        int rhsInt = get<ClauseVisitorConstants::INT_VAL_IDX>(valRHS);
        return count(lhsVecInt.begin(), lhsVecInt.end(), rhsInt);
      }
      if (valRHS.index() == ClauseVisitorConstants::VECTOR_INT_VAL_IDX) {
        vector<int> rhsVecInt =
            get<ClauseVisitorConstants::VECTOR_INT_VAL_IDX>(valRHS);
        return !intersection(lhsVecInt, rhsVecInt).empty();
      }
      break;
    }
  }
  return false;
}

std::vector<std::string> DataPreprocessor::intersection(
    std::vector<std::string> v1, std::vector<std::string> v2) {
  std::vector<std::string> v3;

  std::sort(v1.begin(), v1.end());
  std::sort(v2.begin(), v2.end());

  std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(),
                        back_inserter(v3));
  return v3;
}

std::vector<int> DataPreprocessor::intersection(std::vector<int> v1,
                                                std::vector<int> v2) {
  std::vector<int> v3;

  std::sort(v1.begin(), v1.end());
  std::sort(v2.begin(), v2.end());

  std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(),
                        back_inserter(v3));
  return v3;
}

void QE::DataPreprocessor::getWithValues(
    vector<WithRef> withRefs, const shared_ptr<vector<Value>>& values) {
  for (int i = 0; i < withRefs.size(); i++) {
    if (withRefs[i].index() == WithClause::WITH_REF_IDENT_IDX) {
      values->at(i) = get<WithClause::WITH_REF_IDENT_IDX>(withRefs[i]).identStr;
      continue;
    }
    if (withRefs[i].index() == WithClause::WITH_REF_INT_IDX) {
      values->at(i) = get<WithClause::WITH_REF_INT_IDX>(withRefs[i]);
      continue;
    }
    // withRefs[i].index() == WithClause::WITH_REF_ATTR_REF_IDX
    AttrRef attrRef = std::get<AttrRef>(withRefs[i]);
    DesignEntity designEntity = getDesignEntityOfSyn(attrRef.synonym);

    bool processingNeeded = std::count(entitiesToFilter.begin(),
                                       entitiesToFilter.end(), designEntity) &&
                            std::count(attrsToFilter.begin(),
                                       attrsToFilter.end(), attrRef.attrName);
    if (!processingNeeded) {
      if (attrRef.attrName == AttrName::PROC_NAME) {
        values->at(i) =
            getEntityNames(designEntity);  // design entity must be procedure
        continue;
      }

      if (attrRef.attrName == AttrName::VAR_NAME) {
        values->at(i) =
            getEntityNames(designEntity);  // design entity must be variable
        continue;
      }

      if (attrRef.attrName == AttrName::STMT_NUMBER) {
        values->at(i) = getStmtNumsByDesignEntity(designEntity);  // vector<int>
        continue;
      }
      // attrRef.attrName == AttrName::VALUE
      values->at(i) =
          getEntityNames(designEntity);  // this can only be constant values
                                         // with return type vector<string>
      vector<int> constVals;
      vector<string> strVals =
          get<ClauseVisitorConstants::VECTOR_STR_VAL_IDX>(values->at(i));
      for (const auto& strVal : strVals) {
        constVals.push_back(stoi(strVal));
      }
      values->at(i) = constVals;
    } else {
      if (attrRef.attrName == AttrName::PROC_NAME) {
        values->at(i) = getCallsProcedureTable()->getColumnByName(
            PKBStorage::CALLS_TABLE_COL2_NAME);
      }
      if (attrRef.attrName == QB::AttrName::VAR_NAME &&
          designEntity == QB::DesignEntity::READ) {
        values->at(i) = getReadVariableTable()->getColumnByName(
            PKBStorage::VARIABLE_TABLE_COL1_NAME);
      }
      if (attrRef.attrName == QB::AttrName::VAR_NAME &&
          designEntity == QB::DesignEntity::PRINT) {
        values->at(i) = getPrintVariableTable()->getColumnByName(
            PKBStorage::VARIABLE_TABLE_COL1_NAME);
      }
    }
  }
}

void DataPreprocessor::filerTableByColumnIdx(const shared_ptr<Table>& table,
                                             int colIdx, const string& value) {
  Table filteredTable = Table();
  filteredTable.renameHeader(table->getHeader());
  vector<vector<string>>* rowsPtr = table->getRowsPointer();
  auto rowItr = rowsPtr->begin();
  while (rowItr != rowsPtr->end()) {
    if (rowItr->at(colIdx) == value) {
      ++rowItr;
      continue;
    }
    *rowItr = (*rowsPtr)[table->getNumberOfRows() - 1];
    rowsPtr->erase(rowsPtr->end() - 1);
  }
}

bool DataPreprocessor::dropUnusedColumns(const shared_ptr<Table>& table) {
  bool hasResult = !table->isBodyEmpty();
  bool isDropped = false;
  // drop unused columns
  int offset = 0;
  int n = table->getNumberOfColumns();
  for (int i = 0; i < n; i++) {
    int adjustedIdx = i + offset;
    if (table->getHeader()[adjustedIdx].find(Table::DEFAULT_HEADER_PREFIX) !=
        std::string::npos) {
      table->dropColFromThis(adjustedIdx);
      isDropped = true;
      offset--;
    }
  }
  // return a default response if the original table is not empty but no column
  // is used so all are dropped
  if (table->isBodyEmpty() && hasResult) {
    auto dummy = QEUtils::getScalarResponse(hasResult);
    table->renameHeader(dummy->getHeader());
    for (auto& row : *dummy->getRowsPointer()) {
      table->appendRow(row);
    }
  }
  return isDropped;
}

void DataPreprocessor::filerTableByDesignEntity(const shared_ptr<Table>& table,
                                                int colIdx,
                                                DesignEntity designEntity) {
  vector<DesignEntity> AVAIL_ENTITIES = {
      DesignEntity::STMT, DesignEntity::CONSTANT, DesignEntity::VARIABLE,
      DesignEntity::PROCEDURE};
  if (count(AVAIL_ENTITIES.begin(), AVAIL_ENTITIES.end(), designEntity)) return;
  vector<vector<string>>* rowsPtr = table->getRowsPointer();
  auto rowItr = rowsPtr->begin();

  while (rowItr != rowsPtr->end()) {
    string val = rowItr->at(colIdx);
    DesignEntity entityType =
        this->dataRetriever->getDesignEntityOfStmt(stoi(val));
    if (designEntity == entityType) {
      ++rowItr;
      continue;
    }
    // the stmt in the row does not match the given design entity type， erase
    // the row from the table
    *rowItr = (*rowsPtr)[table->getNumberOfRows() -
                         1];             // overwrite this row with the last row
    rowsPtr->erase(rowsPtr->end() - 1);  // erase the last row
  }
}

}  // namespace QE