//
// Created by Nafour on 31/8/22.
//

#include "QueryEvaluator.h"
#include "query_builder/commons/Query.h"
#include "query_evaluator/QueryResult.h"
#include "TableCombiner.h"
#include "QueryBooleanResult.h"
#include "QueryTupleResult.h"
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
    QueryResult dummyEmptyQueryResult = QueryResult();

    if (query->suchThatClauses->empty() && !query->patternClauses) {//no such that or pattern clause
        return this->evaluateNoConditionQuery(query);
    }
//    Table selectedEntityTable = this->evaluateNoConditionQuery(query).table;
//    selectedEntityTable.renameHeader({query->selectClause->synonym.synonym});

    TableCombiner tableCombiner = TableCombiner();
    Table resultTable;
    for (auto stClause: *query->suchThatClauses) {
        Table intermediateTable = this->dataPreprocessor->getTableByRelation(*stClause);
        if (intermediateTable.isBodyEmpty()) {
            if (query->selectClause->returnType == QB::ReturnType::BOOLEAN) return QueryBooleanResult(false);
            return QueryTupleResult({});
        };
        resultTable = tableCombiner.joinTable( intermediateTable, resultTable);

    }

    if (query->patternClauses) {
        for (auto patternClause: *query->patternClauses) {
            Table intermediateTable = this->dataPreprocessor->getTableByPattern(patternClause);
            if (intermediateTable.isBodyEmpty()) {
                if (query->selectClause->returnType == QB::ReturnType::BOOLEAN) return QueryBooleanResult(false);
                return QueryTupleResult({});
            };
            resultTable = tableCombiner.joinTable( intermediateTable, resultTable);
        }
    }

    shared_ptr<vector<Elem>> returnTuple = query->selectClause->returnResults;
    QueryTupleResult queryResult = QueryTupleResult(returnTuple);
    queryResult.table = resultTable;
//    Synonym toBeSelected = query->selectClause->synonym;
//    QueryResult queryResult = QueryResult();
//    queryResult.colName = toBeSelected.synonym;
//    queryResult.table = resultTable;
    return queryResult;
}

QueryResult QueryEvaluator::evaluateNoConditionQuery(shared_ptr<Query> query) {
    shared_ptr<SelectClause> selectClause = query->selectClause;

    if (selectClause->isBoolean()) {
        return QueryBooleanResult(true);
    }

    shared_ptr<vector<Elem>> returnTuple = selectClause->returnResults;

//
//    Synonym synonym = selectClause->synonym;
//    DesignEntity designEntity = getDesignEntity(synonym, query);
//    Table table = this->dataPreprocessor->getAllByDesignEntity(designEntity);
//
//    table.renameHeader({query->selectClause->synonym.synonym});

    //todo: change data retrieving of entities to query result formatter.
    QueryTupleResult queryResult = QueryTupleResult(returnTuple);
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


