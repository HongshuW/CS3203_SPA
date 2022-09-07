//
// Created by Nafour on 31/8/22.
//

#include "QueryEvaluator.h"
#include "query_builder/commons/Query.h"
#include "query_evaluator/QueryResult.h"
#include "TableCombiner.h"

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
}

QueryEvaluator::QueryEvaluator(shared_ptr<DataPreprocessor> dataPreprocessor) {
    this->dataPreprocessor = dataPreprocessor;
}

QueryResult QueryEvaluator::evaluate(shared_ptr<Query> query) {

    if (query->suchThatClauses->empty()) {
        return this->evaluateNoConditionQuery(query);
    }
    TableCombiner tableCombiner = TableCombiner();
    Table resultTable = Table();
    for (auto stClause: *query->suchThatClauses) {
        Table intermediateTable = this->dataPreprocessor->getTableByRelation(*stClause);
        if (intermediateTable.isBodyEmpty()) return QueryResult();
        resultTable = tableCombiner.joinTable(resultTable, intermediateTable);
    }
    Synonym toBeSelected = query->selectClause->synonym;
    QueryResult queryResult = QueryResult();
    queryResult.colName = toBeSelected.synonym;
    queryResult.table = resultTable;
    return queryResult;
}

QueryResult QueryEvaluator::evaluateNoConditionQuery(shared_ptr<Query> query) {
    shared_ptr<SelectClause> selectClause = query->selectClause;
    Synonym synonym = selectClause->synonym;
    DesignEntity designEntity = getDesignEntity(synonym, query);
    Table table = this->dataPreprocessor->getAllByDesignEntity(designEntity);
    QueryResult queryResult = QueryResult(table);
    queryResult.colName = getDesignEntityColName(designEntity);
    return queryResult;
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


