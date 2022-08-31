//
// Created by Nafour on 31/8/22.
//

#include "QueryEvaluator.h"
#include "query_builder/commons/Query.h"
#include "query_evaluator/QueryResult.h"

using namespace QB;
using namespace QE;

QueryEvaluator::QueryEvaluator(PKB *pkb) {
    this->pkb = pkb;
}

QueryResult QueryEvaluator::evaluate(Query* query) {

    if (query->suchThatClauses->empty()) {
        this->evaluateNoConditionQuery(query);
    }
    return QueryResult();
}

QueryResult QueryEvaluator::evaluateNoConditionQuery(Query *query) {
    SelectClause* selectClause = query->selectClause;
    Synonym synonym = selectClause->synonym;
    DesignEntity designEntity = getDesignEntity(synonym, query);
    switch (designEntity) {
        case DesignEntity::STMT:
            break;
        case DesignEntity::VARIABLE:
            break;
        case DesignEntity::CONSTANT:
            break;
        default:
            break;
    }
    return QueryResult();
}

DesignEntity QueryEvaluator::getDesignEntity(Synonym synonym, Query *query) {
    for (Declaration d: *query->declarations) {
        if (synonym.synonym == d.getSynonym().synonym) {
            return d.getDesignEntity();
        }
    }
    return static_cast<DesignEntity>(NULL);
}


