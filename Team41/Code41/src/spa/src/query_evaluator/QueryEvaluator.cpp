//
// Created by Nafour on 31/8/22.
//

#include "QueryEvaluator.h"

#include <algorithm>
#include <iterator>
#include <numeric>
#include <utility>
#include <vector>

#include "ConcreteClauseVisitor.h"
#include "QEUtils.h"
#include "QueryOptimizer.h"
#include "TableCombiner.h"
#include "constants/ClauseVisitorConstants.h"
#include "constants/QueryEvaluatotConstants.h"
#include "query_builder/clauses/pattern_clauses/IfPatternClause.h"
#include "query_builder/commons/Query.h"

using namespace QB;
using namespace QE;
auto &attrsToFilter = QueryEvaluatotConstants::attrsToFilter;
auto &entitiesToFilter = QueryEvaluatotConstants::entitiesToFilter;

const vector<string> FALSE_RESULT = {"FALSE"};
const vector<string> TRUE_RESULT = {"TRUE"};
const vector<string> EMPTY_RESULT = {};

QueryEvaluator::QueryEvaluator(shared_ptr<DataRetriever> dataRetriever)
    : dataRetriever(std::move(dataRetriever)) {}

vector<string> QueryEvaluator::evaluate(const shared_ptr<Query>& queryPtr) {
  dataRetriever->clearCache();
  this->query = queryPtr;
  this->declarations = queryPtr->declarations;
  shared_ptr<DataPreprocessor> dataPreprocessor =
      make_shared<DataPreprocessor>(dataRetriever, declarations);
  shared_ptr<ConcreteClauseVisitor> concreteClauseVisitor =
      make_shared<ConcreteClauseVisitor>(dataPreprocessor);

  shared_ptr<QueryOptimizer> queryOptimizer =
      make_shared<QueryOptimizer>(queryPtr);
  ConnectedClauseGroups ccg = queryOptimizer->optimise();

  bool isReturnTypeBool =
					queryPtr->selectClause->returnType == QB::ReturnType::BOOLEAN;
  return isReturnTypeBool ? evaluateSelectBoolQuery(concreteClauseVisitor,
                                                    dataPreprocessor, ccg)
                          : evaluateSelectTupleQuery(concreteClauseVisitor,
                                                     dataPreprocessor, ccg);
}

vector<string> QueryEvaluator::evaluateNoConditionSelectTupleQuery(
				const shared_ptr<Query>& sharedPtr, const shared_ptr<ConcreteClauseVisitor>& clauseVisitor) {
  auto resultTable = sharedPtr->selectClause->accept(clauseVisitor);
  vector<string> ans =
      projectResult(resultTable, sharedPtr->selectClause->returnResults);
  return removeDup(ans);
}

DesignEntity QueryEvaluator::getDesignEntity(const Synonym& synonym) {
  for (const Declaration& d : *declarations) {
    if (!(synonym.synonym == d.getSynonym().synonym)) continue;
    return d.getDesignEntity();
  }

  return static_cast<DesignEntity>(NULL);
}

vector<string> QueryEvaluator::removeDup(vector<string> vec) {
  unordered_set<string> ans_set(vec.begin(), vec.end());
  vector<string> ans;
  ans.insert(ans.end(), ans_set.begin(), ans_set.end());
  return ans;
}

vector<string> QueryEvaluator::formatConditionalQueryResult(
				shared_ptr<Table> resultTable, const shared_ptr<vector<Elem>>& tuple,
				const shared_ptr<Query>& sharedPtr, const shared_ptr<DataPreprocessor>& dataPreprocessor) {
  if (sharedPtr->selectClause->returnType == QB::ReturnType::BOOLEAN)
    return resultTable->isBodyEmpty() ? FALSE_RESULT : TRUE_RESULT;

  // cartesian product for any missing synonym
  const int COL_DOES_NOT_EXIST = -1;
  const int FIRST_COL_IDX = 0;
  const int SECOND_COL_IDX = 1;
  for (auto elem : *tuple) {
    if (elem.index() == SelectClause::ELEM_ATTR_REF_IDX) {
      AttrRef attrRef = std::get<AttrRef>(elem);
      int attrNameColIdx = resultTable->getColIdxByName(attrRef.toString());

      if (attrNameColIdx != COL_DOES_NOT_EXIST) continue;

      DesignEntity designEntity = getDesignEntity(attrRef.synonym);
      int synColIdx = resultTable->getColIdxByName(attrRef.synonym.synonym);
      bool filterNeeded = std::count(entitiesToFilter.begin(),
                                     entitiesToFilter.end(), designEntity) &&
                          std::count(attrsToFilter.begin(), attrsToFilter.end(),
                                     attrRef.attrName);

      if (synColIdx == COL_DOES_NOT_EXIST) {
        // this table has only one col
        auto intermediateTable = make_shared<Table>();
        if (filterNeeded) {
          if (designEntity == QB::DesignEntity::CALL)
            intermediateTable = dataPreprocessor->getCallsProcedureTable();
          if (designEntity == QB::DesignEntity::PRINT)
            intermediateTable = dataPreprocessor->getPrintVariableTable();
          if (designEntity == QB::DesignEntity::READ)
            intermediateTable = dataPreprocessor->getReadVariableTable();
          if (intermediateTable->isBodyEmpty()) return EMPTY_RESULT;
          intermediateTable->dropColFromThis(FIRST_COL_IDX);
        } else {
          intermediateTable =
              dataPreprocessor->getAllByDesignEntity(designEntity);
          intermediateTable->dropColFromThis(SECOND_COL_IDX);
          if (intermediateTable->isBodyEmpty()) return EMPTY_RESULT;
        }
        intermediateTable->removeDupRow();
        intermediateTable->renameHeader({attrRef.toString()});
        resultTable =
            TableCombiner().crossProduct(intermediateTable, resultTable);
        if (resultTable->isBodyEmpty()) return EMPTY_RESULT;
      } else {
        // synonym name exists in the table but synonym attribute is not
        if (filterNeeded) {
          auto intermediateTable = make_shared<Table>();
          if (designEntity == QB::DesignEntity::CALL)
            intermediateTable = dataPreprocessor->getCallsProcedureTable();
          if (designEntity == QB::DesignEntity::READ)
            intermediateTable = dataPreprocessor->getReadVariableTable();
          if (designEntity == QB::DesignEntity::PRINT)
            intermediateTable = dataPreprocessor->getPrintVariableTable();
          intermediateTable->renameHeader(
              {attrRef.synonym.synonym, attrRef.toString()});
          resultTable =
              TableCombiner().hashJoin(intermediateTable, resultTable);
        } else {
          resultTable->dupCol(synColIdx, attrRef.toString());
        }
      }
    }
    if (elem.index() == SelectClause::ELEM_SYN_IDX) {
      Synonym synonym = std::get<SelectClause::ELEM_SYN_IDX>(elem);
      // check for existence
      int colIdx = resultTable->getColIdxByName(synonym.synonym);

      if (colIdx == COL_DOES_NOT_EXIST) {
        DesignEntity designEntity = getDesignEntity(synonym);
        auto intermediateTable =
            dataPreprocessor->getAllByDesignEntity(designEntity);
        if (intermediateTable->isBodyEmpty()) return EMPTY_RESULT;

        intermediateTable->renameHeader({synonym.synonym});
        resultTable =
            TableCombiner().crossProduct(intermediateTable, resultTable);
        if (resultTable->isBodyEmpty()) return EMPTY_RESULT;
      }
    }
  }
  // everything should be in the result table, no more cross product and table
  // size stays the same hence can project answers
  return projectResult(resultTable, tuple);
}

vector<string> QueryEvaluator::projectResult(const shared_ptr<Table>& resultTable,
                                             const shared_ptr<vector<Elem>>& tuple) {
  size_t ans_size = resultTable->getNumberOfRows();
  const string EMPTY_STRING;
  vector<string> ans = vector<string>(ans_size, EMPTY_STRING);
  const string SPACE_SEPARATOR = " ";

  for (auto elem : *tuple) {
    if (elem.index() == SelectClause::ELEM_SYN_IDX) {
      Synonym synonym = std::get<SelectClause::ELEM_SYN_IDX>(elem);
      int selectedColIdx = resultTable->getColIdxByName(synonym.synonym);
      vector<string> colValues = resultTable->getColumnByIndex(selectedColIdx);
      for (int r = 0; r < ans_size; r++) {
        ans[r] = ans[r].empty() ? colValues[r]
                                : ans[r] + SPACE_SEPARATOR + colValues[r];
      }
    } else {
      // elem is a attrRef
      AttrRef attrRef = std::get<AttrRef>(elem);
      int selectedColIdx = resultTable->getColIdxByName(attrRef.toString());
      vector<string> colValues = resultTable->getColumnByIndex(selectedColIdx);
      for (int r = 0; r < ans_size; r++) {
        ans[r] = ans[r].empty() ? colValues[r]
                                : ans[r] + SPACE_SEPARATOR + colValues[r];
      }
    }
  }
  dataRetriever->clearCache();
  return removeDup(ans);
}

vector<string> QueryEvaluator::evaluateSelectBoolQuery(
    const shared_ptr<ConcreteClauseVisitor>& clauseVisitor,
    const shared_ptr<DataPreprocessor>& dataPreprocessor, const ConnectedClauseGroups& ccg) {
  // if no condition, return true
  bool hasCondition = !query->suchThatClauses->empty() ||
                      !query->patternClauses->empty() ||
                      !query->withClauses->empty();
  if (!hasCondition) return TRUE_RESULT;

  // eval truthiness of with clauses
  for (const auto& withClause : *query->withClauses) {
    bool doesConditionExist = dataPreprocessor->hasResult(withClause);
    if (!doesConditionExist) return FALSE_RESULT;
  }

  // first evaluate group of clauses without synonyms
  for (const auto& noSynClause : *ccg->at(QueryOptimizer::NO_SYN_GROUP_IDX)) {
    auto intermediateTable = noSynClause->accept(clauseVisitor);
    if (intermediateTable->isBodyEmpty()) return FALSE_RESULT;
  }
  // erase group of clauses without synonyms after evaluation
  ccg->erase(QueryOptimizer::NO_SYN_GROUP_IDX);

  TableCombiner tableCombiner = TableCombiner();
  auto resultTable = QEUtils::getScalarResponse(
      false);  // fill initial table with non-empty false result

  // evaluate the rest of the groups
  for (const auto& it : *ccg) {
    // eval each subgroup
    shared_ptr<Table> subGroupResultTable = make_shared<Table>();
    for (const auto& subGroupClause : *it.second) {
      auto intermediateTable = subGroupClause->accept(clauseVisitor);
      if (intermediateTable->isBodyEmpty()) return FALSE_RESULT;
      subGroupResultTable =
          tableCombiner.hashJoin(intermediateTable, subGroupResultTable);
      if (subGroupResultTable->isBodyEmpty()) return FALSE_RESULT;
    }
    resultTable = tableCombiner.hashJoin(subGroupResultTable, resultTable);
    if (resultTable->isBodyEmpty()) return FALSE_RESULT;
  }
  dataRetriever->clearCache();
  return TRUE_RESULT;
}

vector<string> QueryEvaluator::evaluateSelectTupleQuery(
    const shared_ptr<ConcreteClauseVisitor>& clauseVisitor,
    const shared_ptr<DataPreprocessor>& dataPreprocessor, const ConnectedClauseGroups& ccg) {
  const int SYN_IDX = 0;
  // convert selected elem to string
  for (auto elem : *query->selectClause->returnResults) {
    if (elem.index() == SYN_IDX)
      selected.push_back(get<Synonym>(elem).synonym);
    else {
      selected.push_back(get<AttrRef>(elem).synonym.synonym);
      selected.push_back(get<AttrRef>(elem).toString());
    }
  }
  bool hasCondition = !query->suchThatClauses->empty() ||
                      !query->patternClauses->empty() ||
                      !query->withClauses->empty();
  if (!hasCondition) {  // no conditional clause
    return this->evaluateNoConditionSelectTupleQuery(query, clauseVisitor);
  }

  // evaluate truthiness of with clauses
  for (const auto& withClause : *query->withClauses) {
    bool doesConditionExist = dataPreprocessor->hasResult(withClause);
    if (!doesConditionExist) return EMPTY_RESULT;
  }

  TableCombiner tableCombiner = TableCombiner();
  auto resultTable = QEUtils::getScalarResponse(false);

  // first evaluate group of clauses without synonyms, and don't have to join
  // table
  for (const auto& noSynClause : *ccg->at(QueryOptimizer::NO_SYN_GROUP_IDX)) {
    auto intermediateTable = noSynClause->accept(clauseVisitor);
    if (intermediateTable->isBodyEmpty()) return EMPTY_RESULT;
  }
  ccg->erase(QueryOptimizer::NO_SYN_GROUP_IDX);

  for (const auto& it : *ccg) {
    // eval each subgroup
    auto subGroupResultTable = make_shared<Table>();
    for (const auto& subGroupClause : *it.second) {
      auto intermediateTable = subGroupClause->accept(clauseVisitor);
      if (intermediateTable->isBodyEmpty()) return EMPTY_RESULT;
      subGroupResultTable =
          tableCombiner.hashJoin(intermediateTable, subGroupResultTable);
      if (subGroupResultTable->isBodyEmpty()) return EMPTY_RESULT;
    }
    // no need to join table if headers in the sub result does not appear in the
    // selection
    if (!isInSelect(subGroupResultTable->getHeader())) continue;
    resultTable = tableCombiner.hashJoin(subGroupResultTable, resultTable);
    if (resultTable->isBodyEmpty()) return EMPTY_RESULT;
  }

  shared_ptr<vector<Elem>> returnTuple = query->selectClause->returnResults;
  return formatConditionalQueryResult(resultTable, returnTuple, query,
                                      dataPreprocessor);
}

bool QueryEvaluator::isInSelect(vector<string> headers) {
  // find if there is intersection
  std::vector<std::string> v3;
  std::sort(selected.begin(), selected.end());
  std::sort(headers.begin(), headers.end());
  std::set_intersection(selected.begin(), selected.end(), headers.begin(),
                        headers.end(), back_inserter(v3));
  return !v3.empty();
}
