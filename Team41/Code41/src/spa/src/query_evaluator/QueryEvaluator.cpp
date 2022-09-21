//
// Created by Nafour on 31/8/22.
//

#include "QueryEvaluator.h"
#include "query_builder/commons/Query.h"
#include "query_evaluator/QueryResult.h"
#include "TableCombiner.h"
#include "QueryBooleanResult.h"
#include "QueryTupleResult.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include <numeric>


using namespace QB;
using namespace QE;

namespace QE {
    //TODO: update map
    unordered_map<DesignEntity, std::string> designEntityToColNameMap({
                                                                              {DesignEntity::STMT, "stmt"},
                                                                              {DesignEntity::READ, "read"},
                                                                              {DesignEntity::PRINT, "print"},
                                                                              {DesignEntity::CALL, "call"},
                                                                              {DesignEntity::WHILE, "while"},
                                                                              {DesignEntity::IF, "if"},
                                                                              {DesignEntity::ASSIGN, "assign"},
                                                                              {DesignEntity::VARIABLE, "$variable_name"},
                                                                              {DesignEntity::CONSTANT, "$constant_value"},
                                                                              {DesignEntity::PROCEDURE, "$procedure_name"}
                                                                      });
    const vector<string> FALSE_RESULT = {"FALSE"};
    const vector<string> TRUE_RESULT = {"TRUE"};
    const vector<string> EMPTY_RESULT = {};

}

QueryEvaluator::QueryEvaluator(shared_ptr<DataPreprocessor> dataPreprocessor) {
    this->dataPreprocessor = dataPreprocessor;
}

vector<string> QueryEvaluator::evaluate(shared_ptr<Query> query) {
    bool hasCondition = !query->suchThatClauses->empty() || !query->patternClauses->empty() || !query->withClauses->empty();
    if (!hasCondition) {//no such that or pattern clause
        return this->evaluateNoConditionQuery(query);
    }

    for (auto withClause: *query->withClauses) {
        if (!doesConditionExist(query, *withClause)) {
            if (query->selectClause->returnType == QB::ReturnType::BOOLEAN) return FALSE_RESULT;
            return EMPTY_RESULT;
        };
    }


    TableCombiner tableCombiner = TableCombiner();
    Table resultTable;
    for (auto stClause: *query->suchThatClauses) {
        Table intermediateTable = this->dataPreprocessor->getTableByRelation(*stClause);
        if (intermediateTable.isBodyEmpty()) {
            if (query->selectClause->returnType == QB::ReturnType::BOOLEAN) return FALSE_RESULT;
            return EMPTY_RESULT;

        };
        resultTable = tableCombiner.joinTable( intermediateTable, resultTable);

        if (resultTable.isBodyEmpty()) {
            if (query->selectClause->returnType == QB::ReturnType::BOOLEAN) return FALSE_RESULT;
            return EMPTY_RESULT;
        };

    }

    for (auto patternClause: *query->patternClauses) {
        Table intermediateTable = this->dataPreprocessor->getTableByPattern(patternClause);
        if (intermediateTable.isBodyEmpty()) {
            if (query->selectClause->returnType == QB::ReturnType::BOOLEAN) return FALSE_RESULT;
            return EMPTY_RESULT;
        };
        resultTable = tableCombiner.joinTable( intermediateTable, resultTable);

        if (resultTable.isBodyEmpty()) {
            if (query->selectClause->returnType == QB::ReturnType::BOOLEAN) return FALSE_RESULT;
            return EMPTY_RESULT;
        };
    }

    //all conditions must be valid at this point
    for (auto withClause: *query->withClauses) {
        Table intermediateTable = dataPreprocessor->getTableByWith(withClause, query->declarations);
        resultTable = tableCombiner.joinTable(intermediateTable, resultTable);

        if (resultTable.isBodyEmpty()) {
            if (query->selectClause->returnType == QB::ReturnType::BOOLEAN) return FALSE_RESULT;
            return EMPTY_RESULT;
        };
    }

    shared_ptr<vector<Elem>> returnTuple = query->selectClause->returnResults;
    return formatConditionalQueryResult(resultTable, returnTuple, query);
}

vector<string> QueryEvaluator::evaluateNoConditionQuery(shared_ptr<Query> query) {
    shared_ptr<SelectClause> selectClause = query->selectClause;

    if (selectClause->isBoolean()) {
        return TRUE_RESULT;
    }

    TableCombiner tableCombiner = TableCombiner();
    Table resultTable;
    for (auto elem: *selectClause->returnResults) {
        if (elem.index() == SelectClause::ELEM_SYN_IDX) {
            Synonym synonym = std::get<SelectClause::ELEM_SYN_IDX>(elem);
            DesignEntity designEntity = getDesignEntity(synonym, query);

            Table intermediateTable = dataPreprocessor->getAllByDesignEntity(designEntity);
            if (intermediateTable.isBodyEmpty()) {
                return EMPTY_RESULT;
            }
            intermediateTable.renameHeader({synonym.synonym});
            resultTable = tableCombiner.crossProduct(resultTable, intermediateTable);
        } else {
            //elem is a attrRef
            AttrRef attrRef = std::get<AttrRef>(elem);
            DesignEntity designEntity = getDesignEntity(attrRef.synonym, query);
            Table intermediateTable = dataPreprocessor->getAllByDesignEntity(designEntity);

            if (intermediateTable.isBodyEmpty()) {
                return EMPTY_RESULT;
            }

            vector<DesignEntity> notDirectlyAvailEntities = {DesignEntity::CALL, DesignEntity::READ, DesignEntity::PRINT};
            vector<AttrName> notDirectlyAvailAttrNames = {AttrName::PROC_NAME, AttrName::VAR_NAME};


            bool processingNeeded = std::count(notDirectlyAvailEntities.begin(), notDirectlyAvailEntities.end(), designEntity)
                                    && std::count(notDirectlyAvailAttrNames.begin(), notDirectlyAvailAttrNames.end(), attrRef.attrName);

            //todo: query pkb for data
            if (processingNeeded) {

            } else {
                for (int i = 1; i < intermediateTable.header.size(); i++) {
                    intermediateTable = intermediateTable.dropCol(i);
                }
                string headerName;
                headerName.append("$");
                headerName.append(attrRef.synonym.synonym);
                headerName.append(".");
                headerName.append(attrRef.getStrOfAttrName());
                intermediateTable.renameHeader({headerName});
            }
            resultTable = tableCombiner.crossProduct(resultTable, intermediateTable);
        }
    }

    vector<string> ans;
    for (const auto& row: resultTable.rows) {
        string singleAns = join(row, " ");
        ans.push_back(singleAns);
    }

    return removeDup(ans);
}

DesignEntity QueryEvaluator::getDesignEntity(Synonym synonym, shared_ptr<Query> query) {
    for (Declaration d: *query->declarations) {
        if (synonym.synonym == d.getSynonym().synonym) {
            return d.getDesignEntity();
        }
    }
    return static_cast<DesignEntity>(NULL);
}

string QueryEvaluator::getDesignEntityColName(DesignEntity entity) {
    return designEntityToColNameMap.at(entity);
}

std::string QueryEvaluator::join(std::vector<std::string> const &strings, std::string delim)
{
    if (strings.empty()) {
        return std::string();
    }

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

bool QueryEvaluator::doesConditionExist(shared_ptr<Query> query, WithClause withClause) {

    using Value = variant<string, int, vector<int>, vector<string>>;
    Value valLHS;
    Value valRHS;

    const int STR_VAL_IDX = 0;
    const int INT_VAL_IDX = 1;
    const int VECTOR_INT_VAL_IDX = 2;
    const int VECTOR_STR_VAL_IDX = 3;

    vector<WithRef> withRefs = {withClause.lhs, withClause.rhs};
    vector<Value> values = {valLHS, valRHS};

    //get values from each ref and store in val1 and val2
    for (int i = 0; i < withRefs.size(); i++) {
        switch (withRefs[i].index()) {
            case (WithClause::WITHREF_IDENT_IDX): {
                values[i] = get<WithClause::WITHREF_IDENT_IDX>(withRefs[i]).identStr;
                break;
            }
            case (WithClause::WITHREF_INT_IDX): {
                values[i] = get<WithClause::WITHREF_INT_IDX>(withRefs[i]);
                break;
            }
            case (WithClause::WITHREF_ATTR_REF_IDX): {
                AttrRef attrRef = std::get<AttrRef>(withRefs[i]);
                DesignEntity designEntity = getDesignEntity(attrRef.synonym, query);
                vector<DesignEntity> notDirectlyAvailDEs = {DesignEntity::CALL, DesignEntity::READ, DesignEntity::PRINT};
                vector<AttrName> notDirectlyAvailAttrs = {AttrName::PROC_NAME, AttrName::VAR_NAME};
                bool processingNeeded = std::count(notDirectlyAvailDEs.begin(), notDirectlyAvailDEs.end(), designEntity)
                                        && std::count(notDirectlyAvailAttrs.begin(), notDirectlyAvailAttrs.end(), attrRef.attrName);
                if (!processingNeeded) {
                    switch (attrRef.attrName) {
                        case AttrName::PROC_NAME: {
                            values[i] = dataPreprocessor->getEntityNames(designEntity); //design entity must be procedure
                            break;
                        }
                        case AttrName::VAR_NAME: {
                            values[i] = dataPreprocessor->getEntityNames(designEntity);//design entity must be variable
                            break;
                        }
                        case AttrName::VALUE: {
                            values[i] = dataPreprocessor->getEntityNames(
                                    designEntity);//this can only be constant values with return lhsType vector<string>
                            vector<int> constVals;
                            if (values[i].index() == VECTOR_STR_VAL_IDX) {
                                vector<string> strVals = get<VECTOR_STR_VAL_IDX>(values[i]);
                                for (auto strVal: strVals) {
                                    constVals.push_back(stoi(strVal));
                                }
                            }
                            values[i]= constVals;
                            break;
                        }
                        case AttrName::STMT_NUMBER: {
                            values[i] = dataPreprocessor->getStmtNumsByDesignEntity(designEntity); //vector<int>
                            break;
                        }
                    }
                } else {
                    //todo: process call.procName, read.varname and print.varname
                }
                break;
            }

        }
    }

    //compare two values, assumption made: two values are of the same lhsType, i.e either int or string
    switch (valLHS.index()) {
        case STR_VAL_IDX: {
            string lhsStr = get<STR_VAL_IDX>(valLHS);
            if (valRHS.index() == STR_VAL_IDX) return lhsStr == get<STR_VAL_IDX>(valRHS);
            if (valRHS.index() == VECTOR_STR_VAL_IDX) {
                vector<string> strs = get<VECTOR_STR_VAL_IDX>(valRHS);
                return count(strs.begin(), strs.end(), lhsStr);
            }
            break;
        }
        case INT_VAL_IDX: {
            int lhsInt = get<INT_VAL_IDX>(valLHS);
            if (valRHS.index() == INT_VAL_IDX) return lhsInt == get<INT_VAL_IDX>(valRHS);
            if (valRHS.index() == VECTOR_INT_VAL_IDX) {
                vector<int> ints = get<VECTOR_INT_VAL_IDX>(valRHS);
                return count(ints.begin(), ints.end(), lhsInt);
            }
            break;
        }
        case VECTOR_STR_VAL_IDX: {
            vector<string> lhsVecStr = get<VECTOR_STR_VAL_IDX>(valLHS);
            if (valRHS.index() == STR_VAL_IDX) {
                string rhsStr = get<STR_VAL_IDX>(valRHS);
                return count(lhsVecStr.begin(), lhsVecStr.end(), rhsStr);
            }
            if (valRHS.index() == VECTOR_STR_VAL_IDX) {
                vector<string> rhsVecStr = get<VECTOR_STR_VAL_IDX>(valRHS);
                return !intersection(lhsVecStr, rhsVecStr).empty();
            }
            break;
        }
        case VECTOR_INT_VAL_IDX: {
            vector<int> lhsVecInt = get<VECTOR_INT_VAL_IDX>(valLHS);
            if (valRHS.index() == INT_VAL_IDX) {
                int rhsInt = get<INT_VAL_IDX>(valRHS);
                return count(lhsVecInt.begin(), lhsVecInt.end(), rhsInt);
            }
            if (valRHS.index() == VECTOR_INT_VAL_IDX) {
                vector<int> rhsVecInt = get<VECTOR_INT_VAL_IDX>(valRHS);
                return !intersection(lhsVecInt, rhsVecInt).empty();
            }
            break;
        }
    }

    return false;
}

std::vector<std::string> QueryEvaluator::intersection(std::vector<std::string> v1,
                                      std::vector<std::string> v2){
    std::vector<std::string> v3;

    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    std::set_intersection(v1.begin(),v1.end(),
                          v2.begin(),v2.end(),
                          back_inserter(v3));
    return v3;
}

std::vector<int> QueryEvaluator::intersection(std::vector<int> v1,
                                                      std::vector<int> v2){
    std::vector<int> v3;

    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    std::set_intersection(v1.begin(),v1.end(),
                          v2.begin(),v2.end(),
                          back_inserter(v3));
    return v3;
}

vector<string> QueryEvaluator::formatConditionalQueryResult(Table resultTable, shared_ptr<vector<Elem>> tuple, shared_ptr<Query> query) {
    if (query->selectClause->returnType == QB::ReturnType::BOOLEAN) return resultTable.isBodyEmpty() ? FALSE_RESULT: TRUE_RESULT;

    //cartesian product for any missing synonym
    for (auto elem: *tuple) {
        if (elem.index() == SelectClause::ELEM_ATTR_REF_IDX) {
            AttrRef attrRef = std::get<AttrRef>(elem);
            int colIdx = resultTable.getColIdxByName(attrRef.synonym.synonym);
            if (colIdx == - 1) {
                DesignEntity designEntity = getDesignEntity(attrRef.synonym, query);
                Table intermediateTable = dataPreprocessor->getAllByDesignEntity(designEntity);
                if (intermediateTable.isBodyEmpty()) {
                    return EMPTY_RESULT;
                }
                vector<DesignEntity> notDirectlyAvailEntities = {DesignEntity::CALL, DesignEntity::READ, DesignEntity::PRINT};
                vector<AttrName> notDirectlyAvailAttrNames = {AttrName::PROC_NAME, AttrName::VAR_NAME};
                bool processingNeeded = std::count(notDirectlyAvailEntities.begin(), notDirectlyAvailEntities.end(), designEntity)
                                        && std::count(notDirectlyAvailAttrNames.begin(), notDirectlyAvailAttrNames.end(), attrRef.attrName);
                //todo: query from pkb
                if (processingNeeded) {

                } else {
                    intermediateTable.renameHeader({attrRef.synonym.synonym});
                }
                resultTable = TableCombiner().crossProduct(intermediateTable, resultTable);
                if (resultTable.isBodyEmpty())  return EMPTY_RESULT;
            }
        }
        if (elem.index() == SelectClause::ELEM_SYN_IDX) {
            Synonym synonym = std::get<SelectClause::ELEM_SYN_IDX>(elem);
            int colIdx = resultTable.getColIdxByName(synonym.synonym);

            if (colIdx == -1) {
                DesignEntity designEntity = getDesignEntity(synonym, query);
                Table intermediateTable = dataPreprocessor->getAllByDesignEntity(designEntity);
                if (intermediateTable.isBodyEmpty()) return EMPTY_RESULT;

                intermediateTable.renameHeader({synonym.synonym});
                resultTable = TableCombiner().crossProduct(intermediateTable, resultTable);
                if (resultTable.isBodyEmpty()) return EMPTY_RESULT;
        }
    }}
    //everything should be in the result table, no more cross product and table size stays the same
    //hence can project answers
    size_t ans_size = resultTable.rows.size();
    vector<string> ans = vector<string>(ans_size, "");

    for (auto elem: *tuple) {
        if (elem.index() == SelectClause::ELEM_SYN_IDX) {
            Synonym synonym = std::get<SelectClause::ELEM_SYN_IDX>(elem);
            int colIdx = resultTable.getColIdxByName(synonym.synonym);

            vector<string> colValues = resultTable.getColumnByName(synonym.synonym);
            for (int r = 0; r < ans_size; r++) {
                ans[r] =  ans[r].empty() ? colValues[r] : ans[r] + " " + colValues[r];
            }
        } else {
            //elem is a attrRef
            AttrRef attrRef = std::get<AttrRef>(elem);
            int colIdx = resultTable.getColIdxByName(attrRef.synonym.synonym);
            if (colIdx >= 0) {
                vector<string> colValues = getAttributeValuesOfCol(resultTable, attrRef, query);
                for (int r = 0; r < ans_size; r++) {

                    ans[r] = ans[r].empty() ? colValues[r] : ans[r] + " " + colValues[r];
                }
            }
        }
    }
    return removeDup(ans);
}





vector<string> QueryEvaluator::getAttributeValuesOfCol(Table resultTable, AttrRef attrRef, shared_ptr<Query> query) {
    DesignEntity designEntity = getDesignEntity(attrRef.synonym, query);
    vector<DesignEntity> notDirectlyAvailEntities = {DesignEntity::CALL, DesignEntity::READ, DesignEntity::PRINT};
    vector<AttrName> notDirectlyAvailAttrNames = {AttrName::PROC_NAME, AttrName::VAR_NAME};
    bool processingNeeded = std::count(notDirectlyAvailEntities.begin(), notDirectlyAvailEntities.end(), designEntity)
                            && std::count(notDirectlyAvailAttrNames.begin(), notDirectlyAvailAttrNames.end(), attrRef.attrName);
    vector<string> ans;
    if (processingNeeded) {
        //todo query from pkb
    } else {
        ans = resultTable.getColumnByName(attrRef.synonym.synonym);
    }
    return ans;
}

