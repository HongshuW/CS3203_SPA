//
// Created by Nafour on 31/8/22.
//

#include "QueryEvaluator.h"

#include <algorithm>
#include <iterator>
#include <numeric>
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
    : dataRetriever(dataRetriever) {}

vector<string> QueryEvaluator::evaluate(shared_ptr<Query> query) {
  dataRetriever->clearCache();
  this->query = query;
  this->declarations = query->declarations;
  shared_ptr<DataPreprocessor> dataPreprocessor =
      make_shared<DataPreprocessor>(dataRetriever, declarations);
  shared_ptr<ConcreteClauseVisitor> concreteClauseVisitor =
      make_shared<ConcreteClauseVisitor>(dataPreprocessor);

  shared_ptr<QueryOptimizer> queryOptimizer =
      make_shared<QueryOptimizer>(query);
  ConnectedClauseGroups ccg = queryOptimizer->optimise();

  bool isReturnTypeBool =
      query->selectClause->returnType == QB::ReturnType::BOOLEAN;
  return isReturnTypeBool ? evaluateSelectBoolQuery(concreteClauseVisitor,
                                                    dataPreprocessor, ccg)
                          : evaluateSelectTupleQuery(concreteClauseVisitor,
                                                     dataPreprocessor, ccg);
}

vector<string> QueryEvaluator::evaluateNoConditionSelectTupleQuery(
    shared_ptr<Query> query, shared_ptr<ConcreteClauseVisitor> clauseVisitor) {
  Table resultTable = query->selectClause->accept(clauseVisitor);
  vector<string> ans =
      projectResult(resultTable, query->selectClause->returnResults);
  return removeDup(ans);
}

DesignEntity QueryEvaluator::getDesignEntity(Synonym synonym) {
  for (Declaration d : *declarations) {
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
    Table resultTable, shared_ptr<vector<Elem>> tuple, shared_ptr<Query> query,
    shared_ptr<DataPreprocessor> dataPreprocessor) {
  if (query->selectClause->returnType == QB::ReturnType::BOOLEAN)
    return resultTable.isBodyEmpty() ? FALSE_RESULT : TRUE_RESULT;

  // cartesian product for any missing synonym
  const int COL_DOES_NOT_EXIST = -1;
  const int FIRST_COL_IDX = 0;
  const int SECOND_COL_IDX = 1;
  for (auto elem : *tuple) {
    if (elem.index() == SelectClause::ELEM_ATTR_REF_IDX) {
      AttrRef attrRef = std::get<AttrRef>(elem);
      int attrNameColIdx = resultTable.getColIdxByName(attrRef.toString());

      if (attrNameColIdx != COL_DOES_NOT_EXIST) continue;

      DesignEntity designEntity = getDesignEntity(attrRef.synonym);
      int synColIdx = resultTable.getColIdxByName(attrRef.synonym.synonym);
      bool filterNeeded = std::count(entitiesToFilter.begin(),
                                     entitiesToFilter.end(), designEntity) &&
                          std::count(attrsToFilter.begin(), attrsToFilter.end(),
                                     attrRef.attrName);

      if (synColIdx == COL_DOES_NOT_EXIST) {
        // this table has only one col
        Table intermediateTable;
        if (filterNeeded) {
          if (designEntity == QB::DesignEntity::CALL)
            intermediateTable = dataPreprocessor->getCallsProcedureTable();
          if (designEntity == QB::DesignEntity::PRINT)
            intermediateTable = dataPreprocessor->getPrintVariableTable();
          if (designEntity == QB::DesignEntity::READ)
            intermediateTable = dataPreprocessor->getReadVariableTable();
          if (intermediateTable.isBodyEmpty()) return EMPTY_RESULT;
          intermediateTable = intermediateTable.dropCol(FIRST_COL_IDX);
        } else {
          intermediateTable =
              dataPreprocessor->getAllByDesignEntity(designEntity)
                  .dropCol(SECOND_COL_IDX);
          if (intermediateTable.isBodyEmpty()) return EMPTY_RESULT;
        }

        intermediateTable.renameHeader({attrRef.toString()});
        resultTable =
            TableCombiner().crossProduct(intermediateTable, resultTable);
        if (resultTable.isBodyEmpty()) return EMPTY_RESULT;
      } else {
        // synonym name exists in the table but synonym attribute is not
        if (filterNeeded) {
          Table intermediateTable;
          if (designEntity == QB::DesignEntity::CALL)
            intermediateTable = dataPreprocessor->getCallsProcedureTable();
          if (designEntity == QB::DesignEntity::READ)
            intermediateTable = dataPreprocessor->getReadVariableTable();
          if (designEntity == QB::DesignEntity::PRINT)
            intermediateTable = dataPreprocessor->getPrintVariableTable();
          intermediateTable.renameHeader(
              {attrRef.synonym.synonym, attrRef.toString()});
          resultTable =
              TableCombiner().joinTable(intermediateTable, resultTable);
        } else {
          resultTable = resultTable.dupCol(synColIdx, attrRef.toString());
        }
      }
    }
    if (elem.index() == SelectClause::ELEM_SYN_IDX) {
      Synonym synonym = std::get<SelectClause::ELEM_SYN_IDX>(elem);
      // check for existence
      int colIdx = resultTable.getColIdxByName(synonym.synonym);

      if (colIdx == COL_DOES_NOT_EXIST) {
        DesignEntity designEntity = getDesignEntity(synonym);
        Table intermediateTable =
            dataPreprocessor->getAllByDesignEntity(designEntity);
        if (intermediateTable.isBodyEmpty()) return EMPTY_RESULT;

        intermediateTable.renameHeader({synonym.synonym});
        resultTable =
            TableCombiner().crossProduct(intermediateTable, resultTable);
        if (resultTable.isBodyEmpty()) return EMPTY_RESULT;
      }
    }
  }
  // everything should be in the result table, no more cross product and table
  // size stays the same hence can project answers
  return projectResult(resultTable, tuple);
}

vector<string> QueryEvaluator::projectResult(Table resultTable,
                                             shared_ptr<vector<Elem>> tuple) {
  size_t ans_size = resultTable.rows.size();
  const string EMPTY_STRING = "";
  vector<string> ans = vector<string>(ans_size, EMPTY_STRING);
  const string SPACE_SEPARATOR = " ";

  for (auto elem : *tuple) {
    if (elem.index() == SelectClause::ELEM_SYN_IDX) {
      Synonym synonym = std::get<SelectClause::ELEM_SYN_IDX>(elem);
      int selectedColIdx = resultTable.getColIdxByName(synonym.synonym);
      vector<string> colValues = resultTable.getColumnByIndex(selectedColIdx);
      for (int r = 0; r < ans_size; r++) {
        ans[r] = ans[r].empty() ? colValues[r]
                                : ans[r] + SPACE_SEPARATOR + colValues[r];
      }
    } else {
      // elem is a attrRef
      AttrRef attrRef = std::get<AttrRef>(elem);
      int selectedColIdx = resultTable.getColIdxByName(attrRef.toString());
      vector<string> colValues = resultTable.getColumnByIndex(selectedColIdx);
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
    shared_ptr<ConcreteClauseVisitor> clauseVisitor,
    shared_ptr<DataPreprocessor> dataPreprocessor, ConnectedClauseGroups ccg) {
  // if no condition, return true
  bool hasCondition = !query->suchThatClauses->empty() ||
                      !query->patternClauses->empty() ||
                      !query->withClauses->empty();
  if (!hasCondition) return TRUE_RESULT;

  // eval truthiness of with clauses
  for (auto withClause : *query->withClauses) {
    bool doesConditionExist = dataPreprocessor->hasResult(withClause);
    if (!doesConditionExist) return FALSE_RESULT;
  }

  // first evaluate group of clauses without synonyms
  for (auto noSynClause : *ccg->at(QueryOptimizer::NO_SYN_GROUP_IDX)) {
    Table intermediateTable = noSynClause->accept(clauseVisitor);
    if (intermediateTable.isBodyEmpty()) return FALSE_RESULT;
  }
  // erase group of clauses without synonyms after evaluation
  ccg->erase(QueryOptimizer::NO_SYN_GROUP_IDX);

  TableCombiner tableCombiner = TableCombiner();
  Table resultTable = QEUtils::getScalarResponse(
      false);  // fill initial table with non-empty false result

  // evaluate the rest of the groups
  for (auto it : *ccg) {
    // eval each subgroup
    Table subGroupResultTable;
    for (auto subGroupClause : *it.second) {
      Table intermediateTable = subGroupClause->accept(clauseVisitor);
      if (intermediateTable.isBodyEmpty()) return FALSE_RESULT;
      subGroupResultTable =
          tableCombiner.joinTable(intermediateTable, subGroupResultTable);
      if (subGroupResultTable.isBodyEmpty()) return FALSE_RESULT;
    }
    resultTable = tableCombiner.joinTable(subGroupResultTable, resultTable);
    if (resultTable.isBodyEmpty()) return FALSE_RESULT;
  }
  dataRetriever->clearCache();
  return TRUE_RESULT;
}

vector<string> QueryEvaluator::evaluateSelectTupleQuery(
    shared_ptr<ConcreteClauseVisitor> clauseVisitor,
    shared_ptr<DataPreprocessor> dataPreprocessor, ConnectedClauseGroups ccg) {
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
  for (auto withClause : *query->withClauses) {
    bool doesConditionExist = dataPreprocessor->hasResult(withClause);
    if (!doesConditionExist) return EMPTY_RESULT;
  }

  TableCombiner tableCombiner = TableCombiner();
  Table resultTable = QEUtils::getScalarResponse(false);

  // first evaluate group of clauses without synonyms, and dont have to join
  // table
  for (auto noSynClause : *ccg->at(QueryOptimizer::NO_SYN_GROUP_IDX)) {
    Table intermediateTable = noSynClause->accept(clauseVisitor);
    if (intermediateTable.isBodyEmpty()) return EMPTY_RESULT;
  }
  ccg->erase(QueryOptimizer::NO_SYN_GROUP_IDX);

  for (auto it : *ccg) {
    // eval each subgroup
    Table subGroupResultTable;
    for (auto subGroupClause : *it.second) {
      Table intermediateTable = subGroupClause->accept(clauseVisitor);
      if (intermediateTable.isBodyEmpty()) return EMPTY_RESULT;
      subGroupResultTable =
          tableCombiner.joinTable(intermediateTable, subGroupResultTable);
      if (subGroupResultTable.isBodyEmpty()) return EMPTY_RESULT;
    }
    // no need to join table if headers in the sub result does not appear in the
    // selection
    if (!isInSelect(subGroupResultTable.header)) continue;
    resultTable = tableCombiner.joinTable(subGroupResultTable, resultTable);
    if (resultTable.isBodyEmpty()) return EMPTY_RESULT;
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
