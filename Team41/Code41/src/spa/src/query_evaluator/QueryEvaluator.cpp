//
// Created by Nafour on 31/8/22.
//

#include "QueryEvaluator.h"
#include "query_builder/commons/Query.h"
#include "TableCombiner.h"
#include <algorithm>
#include <vector>
#include <iterator>
#include <numeric>
#include "constants/ClauseVisitorConstants.h"
#include "query_builder/clauses/pattern_clauses/IfPatternClause.h"
#include "ConcreteClauseVisitor.h"
#include "QueryOptimizer.h"

using namespace QB;
using namespace QE;

const vector<string> FALSE_RESULT = {"FALSE"};
const vector<string> TRUE_RESULT = {"TRUE"};
const vector<string> EMPTY_RESULT = {};

QueryEvaluator::QueryEvaluator(shared_ptr<DataRetriever> dataRetriever): dataRetriever(dataRetriever) {

}

vector<string> QueryEvaluator::evaluate(shared_ptr<Query> query) {
    this->query = query;
    this->declarations = query->declarations;
    shared_ptr<DataPreprocessor> dataPreprocessor = make_shared<DataPreprocessor>(dataRetriever, declarations);
//    shared_ptr<ClauseVisitor> clauseEvaluator = make_shared<ClauseVisitor>(dataPreprocessor,
//                                                                           declarations);
    shared_ptr<ConcreteClauseVisitor> concreteClauseVisitor = make_shared<ConcreteClauseVisitor>(dataPreprocessor);

    shared_ptr<QueryOptimizer> queryOptimizer = make_shared<QueryOptimizer>(query);
    ConnectedClauseGroups ccg = queryOptimizer->optimise();

    bool isReturnTypeBool = query->selectClause->returnType == QB::ReturnType::BOOLEAN;
    return isReturnTypeBool
    ? evaluateSelectBoolQuery(concreteClauseVisitor, dataPreprocessor, ccg)
    : evaluateSelectTupleQuery(concreteClauseVisitor, dataPreprocessor, ccg);
}

vector<string>
QueryEvaluator::evaluateNoConditionSelectTupleQuery(shared_ptr<Query> query, shared_ptr<ConcreteClauseVisitor> clauseVisitor) {
    Table resultTable = query->selectClause->accept(clauseVisitor);
    vector<string> ans = projectResult(resultTable, query->selectClause->returnResults);
    return removeDup(ans);
}

DesignEntity QueryEvaluator::getDesignEntity(Synonym synonym) {
    for (Declaration d: *declarations) {
        if (!(synonym.synonym == d.getSynonym().synonym)) continue;
        return d.getDesignEntity();
    }

    return static_cast<DesignEntity>(NULL);
}

std::string QueryEvaluator::join(std::vector<std::string> const &strings, std::string delim)
{
    if (strings.empty()) return std::string();

    return std::accumulate(strings.begin() + 1, strings.end(), strings[0],
                           [&delim](std::string x, std::string y) {
                               return x + delim + y;
                           }
    );
}

vector<string> QueryEvaluator::removeDup(vector<string> vec) {
    unordered_set<string> ans_set(vec.begin(), vec.end());
    vector<string> ans;
    ans.insert(ans.end(), ans_set.begin(), ans_set.end());
    return ans;
}

vector<string>
QueryEvaluator::formatConditionalQueryResult(Table resultTable, shared_ptr<vector<Elem>> tuple, shared_ptr<Query> query,
                                             shared_ptr<DataPreprocessor> dataPreprocessor) {
    if (query->selectClause->returnType == QB::ReturnType::BOOLEAN) return resultTable.isBodyEmpty() ? FALSE_RESULT: TRUE_RESULT;

    //cartesian product for any missing synonym
    const int COL_DOES_NOT_EXIST = -1;
    const int FIRST_COL_IDX = 0;
    const int SECOND_COL_IDX = 1;
    for (auto elem: *tuple) {
        if (elem.index() == SelectClause::ELEM_ATTR_REF_IDX) {
            AttrRef attrRef = std::get<AttrRef>(elem);
            int attrNameColIdx = resultTable.getColIdxByName(attrRef.toString());

            if (attrNameColIdx != COL_DOES_NOT_EXIST) continue;

            DesignEntity designEntity = getDesignEntity(attrRef.synonym);
            int synColIdx = resultTable.getColIdxByName(attrRef.synonym.synonym);

            if (synColIdx == COL_DOES_NOT_EXIST) {
                //this table has only one col
                Table intermediateTable = dataPreprocessor->getAllByDesignEntity(designEntity);

                if (intermediateTable.isBodyEmpty()) return EMPTY_RESULT;
                vector<DesignEntity> notDirectlyAvailEntities = {DesignEntity::CALL, DesignEntity::READ, DesignEntity::PRINT};
                vector<AttrName> notDirectlyAvailAttrNames = {AttrName::PROC_NAME, AttrName::VAR_NAME};
                bool processingNeeded = std::count(notDirectlyAvailEntities.begin(), notDirectlyAvailEntities.end(), designEntity)
                                        && std::count(notDirectlyAvailAttrNames.begin(), notDirectlyAvailAttrNames.end(), attrRef.attrName);

                if (processingNeeded) {
                    if (designEntity == QB::DesignEntity::CALL) intermediateTable = dataPreprocessor->getCallsProcedureTable();
                    if (designEntity == QB::DesignEntity::PRINT) intermediateTable = dataPreprocessor->getPrintVariableTable();
                    if (designEntity == QB::DesignEntity::READ) intermediateTable = dataPreprocessor->getReadVariableTable();
                    if (intermediateTable.isBodyEmpty()) return EMPTY_RESULT;
                    intermediateTable = intermediateTable.dropCol(FIRST_COL_IDX);
                } else {

                    intermediateTable = intermediateTable.dropCol(SECOND_COL_IDX);
                }

                intermediateTable.renameHeader({attrRef.toString()});

                resultTable = TableCombiner().crossProduct(intermediateTable, resultTable);
                if (resultTable.isBodyEmpty())  return EMPTY_RESULT;
            } else {
                //synonym name exists in the table but synonym attribute is not
                const vector<DesignEntity> entitiesToFilter = {DesignEntity::CALL, DesignEntity::READ, DesignEntity::PRINT};
                bool needFilter = std::find(entitiesToFilter.begin(), entitiesToFilter.end(), designEntity) != entitiesToFilter.end();
                if (needFilter) {
                   Table intermediateTable;
                   if (designEntity == QB::DesignEntity::CALL) intermediateTable = dataPreprocessor->getCallsProcedureTable();
                   if (designEntity == QB::DesignEntity::READ) intermediateTable = dataPreprocessor->getReadVariableTable();
                   if (designEntity == QB::DesignEntity::PRINT) intermediateTable = dataPreprocessor->getPrintVariableTable();
                   intermediateTable.renameHeader({attrRef.synonym.synonym, attrRef.toString()});
                   resultTable = TableCombiner().joinTable(intermediateTable, resultTable);
                } else {
                    resultTable = resultTable.dupCol(synColIdx, attrRef.toString());
                }
            }
        }
        if (elem.index() == SelectClause::ELEM_SYN_IDX) {
            Synonym synonym = std::get<SelectClause::ELEM_SYN_IDX>(elem);
            //check for existence
            int colIdx = resultTable.getColIdxByName(synonym.synonym);

            if (colIdx == COL_DOES_NOT_EXIST) {
                DesignEntity designEntity = getDesignEntity(synonym);
                Table intermediateTable = dataPreprocessor->getAllByDesignEntity(designEntity);
                if (intermediateTable.isBodyEmpty()) return EMPTY_RESULT;

                intermediateTable.renameHeader({synonym.synonym});
                resultTable = TableCombiner().crossProduct(intermediateTable, resultTable);
                if (resultTable.isBodyEmpty()) return EMPTY_RESULT;
        }
    }}
    //everything should be in the result table, no more cross product and table size stays the same
    //hence can project answers
    return projectResult(resultTable, tuple);
}

vector<string> QueryEvaluator::projectResult(Table resultTable, shared_ptr<vector<Elem>> tuple) {
    size_t ans_size = resultTable.rows.size();
    vector<string> ans = vector<string>(ans_size, "");
    ViewedDups viewedDupsMap = make_shared<unordered_map<string, shared_ptr<unordered_set<int>>>>();

    for (auto elem: *tuple) {
        if (elem.index() == SelectClause::ELEM_SYN_IDX) {
            Synonym synonym = std::get<SelectClause::ELEM_SYN_IDX>(elem);
            int selectedColIdx = resultTable.getColIdxByName(synonym.synonym);
            vector<string> colValues = resultTable.getColumnByIndex(selectedColIdx);
            for (int r = 0; r < ans_size; r++) {
                ans[r] =  ans[r].empty() ? colValues[r] : ans[r] + " " + colValues[r];
            }
        } else {
            //elem is a attrRef
            AttrRef attrRef = std::get<AttrRef>(elem);
            int selectedColIdx = resultTable.getColIdxByName(attrRef.toString());
            vector<string> colValues = resultTable.getColumnByIndex(selectedColIdx);
            for (int r = 0; r < ans_size; r++) {
                ans[r] = ans[r].empty() ? colValues[r] : ans[r] + " " + colValues[r];
            }
        }
    }
    return removeDup(ans);
}

int QueryEvaluator::getUnvisitedColIdxByName(const string& colName, ViewedDups viewedDupsMap, const Table& table) {
    for (int i = 0; i < table.header.size(); i++) {
        if (viewedDupsMap->find(colName) == viewedDupsMap->end()) {
            auto set = make_shared<unordered_set<int>>();
            viewedDupsMap->insert({colName, set});
        }
        if (table.header[i] == colName && !viewedDupsMap->at(colName)->count(i)) {
            viewedDupsMap->at(colName)->insert(i);
            return i;
        }
    }
    return -1;
}

vector<string> QueryEvaluator::evaluateSelectBoolQuery(shared_ptr<ConcreteClauseVisitor> clauseVisitor,
                                                       shared_ptr<DataPreprocessor> dataPreprocessor,
                                                       ConnectedClauseGroups ccg) {
    //if no condition, return true
    bool hasCondition = !query->suchThatClauses->empty() || !query->patternClauses->empty() || !query->withClauses->empty();
    if (!hasCondition) return TRUE_RESULT;

    //eval truthiness of with clauses
    for (auto withClause: *query->withClauses) {
        bool doesConditionExist = dataPreprocessor->hasResult(withClause);
        if (!doesConditionExist) return FALSE_RESULT;
    }

    //first evaluate group of clauses without synonyms
    for (auto noSynClause: *ccg->at(QueryOptimizer::NO_SYN_GROUP_IDX)) {
        Table intermediateTable = noSynClause->accept(clauseVisitor);
        if (intermediateTable.isBodyEmpty()) return  FALSE_RESULT;
    }
    //erase group of clauses without synonyms after evaluation
    ccg->erase(QueryOptimizer::NO_SYN_GROUP_IDX);

    TableCombiner tableCombiner = TableCombiner();
    const string DUMMY_HEADER = "$dummy_header";
    const string DUMMY_VALUE = "$dummy_value";
    Table resultTable = Table();
    resultTable.renameHeader({DUMMY_HEADER}) ;
    resultTable.rows = vector<vector<string>>({{DUMMY_VALUE}});

    //evaluate the rest of the groups
    for (auto it: *ccg) {
        //eval each subgroup
        Table subGroupResultTable;
        for (auto subGroupClause: *it.second) {
            Table intermediateTable = subGroupClause->accept(clauseVisitor);
            if (intermediateTable.isBodyEmpty()) return FALSE_RESULT ;
            subGroupResultTable = tableCombiner.joinTable( intermediateTable, subGroupResultTable);
            if (subGroupResultTable.isBodyEmpty()) return FALSE_RESULT;
        }
        resultTable = tableCombiner.joinTable(subGroupResultTable, resultTable);
        if (resultTable.isBodyEmpty()) return FALSE_RESULT;
    }

    return TRUE_RESULT;
}

vector<string> QueryEvaluator::evaluateSelectTupleQuery( shared_ptr<ConcreteClauseVisitor> clauseVisitor, shared_ptr<DataPreprocessor> dataPreprocessor, ConnectedClauseGroups ccg) {

    bool hasCondition = !query->suchThatClauses->empty() || !query->patternClauses->empty() || !query->withClauses->empty();
    if (!hasCondition) {//no conditional clause
        return this->evaluateNoConditionSelectTupleQuery(query, clauseVisitor);
    }

    //evaluate truthiness of with clauses
    for (auto withClause: *query->withClauses) {
        bool doesConditionExist = dataPreprocessor->hasResult(withClause);
        if (!doesConditionExist) return EMPTY_RESULT;
    }

    TableCombiner tableCombiner = TableCombiner();
    const string DUMMY_HEADER = "$dummy_header";
    const string DUMMY_VALUE = "$dummy_value";
    Table resultTable = Table();
    resultTable.renameHeader({DUMMY_HEADER}) ;
    resultTable.rows = vector<vector<string>>({{DUMMY_VALUE}});

    //first evaluate group of clauses without synonyms, and dont have to join table
    for (auto noSynClause: *ccg->at(QueryOptimizer::NO_SYN_GROUP_IDX)) {
        Table intermediateTable = noSynClause->accept(clauseVisitor);
        if (intermediateTable.isBodyEmpty()) return EMPTY_RESULT;
    }
    ccg->erase(QueryOptimizer::NO_SYN_GROUP_IDX);

    for (auto it: *ccg) {
        //eval each subgroup
        Table subGroupResultTable;
        for (auto subGroupClause: *it.second) {
            Table intermediateTable = subGroupClause->accept(clauseVisitor);
            if (intermediateTable.isBodyEmpty()) return EMPTY_RESULT;
            subGroupResultTable = tableCombiner.joinTable( intermediateTable, subGroupResultTable);
            if (subGroupResultTable.isBodyEmpty()) return EMPTY_RESULT;
        }
        resultTable = tableCombiner.joinTable(subGroupResultTable, resultTable);
        if (resultTable.isBodyEmpty()) return EMPTY_RESULT;
    }

    shared_ptr<vector<Elem>> returnTuple = query->selectClause->returnResults;
    return formatConditionalQueryResult(resultTable, returnTuple, query, dataPreprocessor);
}



