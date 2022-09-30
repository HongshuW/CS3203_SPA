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
#include "ClauseVisitor.h"
#include "constants/ClauseVisitorConstants.h"
#include "query_builder/clauses/pattern_clauses/IfPatternClause.h"
#include "ConcreteClauseVisitor.h"

using namespace QB;
using namespace QE;

const vector<string> FALSE_RESULT = {"FALSE"};
const vector<string> TRUE_RESULT = {"TRUE"};
const vector<string> EMPTY_RESULT = {};

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


}

QueryEvaluator::QueryEvaluator(shared_ptr<DataRetriever> dataRetriever): dataRetriever(dataRetriever) {

}

vector<string> QueryEvaluator::evaluate(shared_ptr<Query> query) {
    this->declarations = query->declarations;
    shared_ptr<DataPreprocessor> dataPreprocessor = make_shared<DataPreprocessor>(dataRetriever, declarations);
    shared_ptr<ClauseVisitor> clauseEvaluator = make_shared<ClauseVisitor>(dataPreprocessor,
                                                                           declarations);


    bool hasCondition = !query->suchThatClauses->empty() || !query->patternClauses->empty() || !query->withClauses->empty();
    if (!hasCondition) {//no such that or pattern clause
        return this->evaluateNoConditionQuery(query, clauseEvaluator);
    }
    bool isReturnTypeBool = query->selectClause->returnType == QB::ReturnType::BOOLEAN;

    clauseEvaluator->setReturnBool();
    for (auto withClause: *query->withClauses) {
        bool doesConditionExist = visit(*clauseEvaluator, (Clause) withClause).isEqual(
                ClauseVisitorConstants::FALSE_TABLE);
        if (!doesConditionExist) return isReturnTypeBool ? FALSE_RESULT : EMPTY_RESULT;
    }

    clauseEvaluator->setReturnTable();
    TableCombiner tableCombiner = TableCombiner();
    Table resultTable;
    for (Clause stClause: *query->suchThatClauses) {
        Table intermediateTable = std::visit(*clauseEvaluator, stClause);
        if (intermediateTable.isBodyEmpty()) return isReturnTypeBool ? FALSE_RESULT : EMPTY_RESULT;
        resultTable = tableCombiner.joinTable( intermediateTable, resultTable);
        if (resultTable.isBodyEmpty()) return isReturnTypeBool ? FALSE_RESULT : EMPTY_RESULT;
    }

    shared_ptr<ConcreteClauseVisitor> concreteClauseVisitor = make_shared<ConcreteClauseVisitor>(dataPreprocessor);

    for (auto patternClause: *query->patternClauses) {

        Table intermediateTable = patternClause->accept(concreteClauseVisitor);;
        if (intermediateTable.isBodyEmpty()) return isReturnTypeBool ? FALSE_RESULT : EMPTY_RESULT;
        resultTable = tableCombiner.joinTable( intermediateTable, resultTable);
        if (resultTable.isBodyEmpty()) return isReturnTypeBool ? FALSE_RESULT : EMPTY_RESULT;
    }

    //all conditions must be valid at this point
    for (Clause withClause: *query->withClauses) {
        Table intermediateTable = std::visit(*clauseEvaluator, withClause);
        resultTable = tableCombiner.joinTable(intermediateTable, resultTable);
        if (resultTable.isBodyEmpty()) return isReturnTypeBool ? FALSE_RESULT : EMPTY_RESULT;
    }

    shared_ptr<vector<Elem>> returnTuple = query->selectClause->returnResults;
    return formatConditionalQueryResult(resultTable, returnTuple, query, dataPreprocessor);
}

vector<string>
QueryEvaluator::evaluateNoConditionQuery(shared_ptr<Query> query, shared_ptr<ClauseVisitor> clauseVisitor) {
    shared_ptr<SelectClause> selectClause = query->selectClause;

    if (selectClause->isBoolean()) return TRUE_RESULT;

    Table resultTable = std::visit(*clauseVisitor, (Clause) selectClause);
    vector<string> ans;

    for (const auto& row: resultTable.rows) {
        string singleAns = join(row, " ");
        ans.push_back(singleAns);
    }
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
    for (auto elem: *tuple) {
        if (elem.index() == SelectClause::ELEM_ATTR_REF_IDX) {
            AttrRef attrRef = std::get<AttrRef>(elem);
            int colIdx = resultTable.getColIdxByName(attrRef.synonym.synonym);
            if (colIdx == COL_DOES_NOT_EXIST) {
                DesignEntity designEntity = getDesignEntity(attrRef.synonym);
                Table intermediateTable = dataPreprocessor->getAllByDesignEntity(designEntity);
                if (intermediateTable.isBodyEmpty()) return EMPTY_RESULT;
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

vector<string> QueryEvaluator::getAttributeValuesOfCol(Table resultTable, AttrRef attrRef) {
    DesignEntity designEntity = getDesignEntity(attrRef.synonym);
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

vector<string> QueryEvaluator::projectResult(Table resultTable, shared_ptr<vector<Elem>> tuple) {
    size_t ans_size = resultTable.rows.size();
    vector<string> ans = vector<string>(ans_size, "");

    for (auto elem: *tuple) {
        if (elem.index() == SelectClause::ELEM_SYN_IDX) {
            Synonym synonym = std::get<SelectClause::ELEM_SYN_IDX>(elem);
            vector<string> colValues = resultTable.getColumnByName(synonym.synonym);
            for (int r = 0; r < ans_size; r++) {
                ans[r] =  ans[r].empty() ? colValues[r] : ans[r] + " " + colValues[r];
            }
        } else {
            //elem is a attrRef
            AttrRef attrRef = std::get<AttrRef>(elem);
            int colIdx = resultTable.getColIdxByName(attrRef.synonym.synonym);
            if (colIdx >= 0) {
                vector<string> colValues = getAttributeValuesOfCol(resultTable, attrRef);
                for (int r = 0; r < ans_size; r++) {
                    ans[r] = ans[r].empty() ? colValues[r] : ans[r] + " " + colValues[r];
                }
            }
        }
    }
    return removeDup(ans);
}

