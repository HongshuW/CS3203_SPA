//
// Created by hongshu wang on 31/8/22.
//

#include "DataRetriever.h"

DataRetriever::DataRetriever(shared_ptr<PKBStorage> pkbStorage) {
    this->pkbStorage = pkbStorage;
}

Table DataRetriever::getVariables() {
    return *pkbStorage->getVariables();
}

Table DataRetriever::getTableByDesignEntity(DesignEntity designEntity) {
    switch (designEntity) {
        case DesignEntity::PROCEDURE:
            return *pkbStorage->getProcedures();
        case DesignEntity::STMT:
            return *pkbStorage->getStatements();
        case DesignEntity::VARIABLE:
            return *pkbStorage->getVariables();
        case DesignEntity::CONSTANT:
            return *pkbStorage->getConstants();
        default:
            return Table();
    }
}

Table DataRetriever::getTableByRelationType(RelationType relationType) {
    switch (relationType) {
        case RelationType::FOLLOWS:
            return *pkbStorage->getFollows();
        case RelationType::FOLLOWS_T:
            return *pkbStorage->getFollowsT();
        case RelationType::PARENT:
            return *pkbStorage->getParent();
        case RelationType::PARENT_T:
            return *pkbStorage->getParentT();
        case RelationType::MODIFIES_S:
            return *pkbStorage->getModifiesS();
        case RelationType::MODIFIES_P:
            return *pkbStorage->getModifiesP();
        case RelationType::USES_S:
            return *pkbStorage->getUsesS();
        case RelationType::USES_P:
            return *pkbStorage->getUsesP();
        default:
            return Table();
    }
}

Table DataRetriever::getTableByPattern(ExpressionSpec expressionSpec) {
    return *pkbStorage->getMatchedPatterns(expressionSpec);
}

DesignEntity DataRetriever::getDesignEntityOfStmt(int stmtNumber) {
    string stmtNumberString = to_string(stmtNumber);
    string type = pkbStorage->getStmtType(stmtNumberString);
    return getDesignEntity(type);
}
