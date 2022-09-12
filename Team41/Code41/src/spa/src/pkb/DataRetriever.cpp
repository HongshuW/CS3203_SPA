//
// Created by hongshu wang on 31/8/22.
//

#include "DataRetriever.h"
#include "RelationshipManager.h"
#include "EntityManager.h"
#include "PatternManager.h"

Table DataRetriever::getVariables() {
    return *EntityManager::getVariables();
}

Table DataRetriever::getTableByDesignEntity(DesignEntity designEntity) {
    switch (designEntity) {
        case DesignEntity::PROCEDURE:
            return *EntityManager::getProcedures();
        case DesignEntity::STMT:
            return *EntityManager::getStatements();
        case DesignEntity::VARIABLE:
            return *EntityManager::getVariables();
        case DesignEntity::CONSTANT:
            return *EntityManager::getConstants();
        default:
            return Table();
    }
}

Table DataRetriever::getTableByRelationType(RelationType relationType) {
    switch (relationType) {
        case RelationType::FOLLOWS:
            return *RelationshipManager::getFollows();
        case RelationType::FOLLOWS_T:
            return *RelationshipManager::getFollowsT();
        case RelationType::PARENT:
            return *RelationshipManager::getParent();
        case RelationType::PARENT_T:
            return *RelationshipManager::getParentT();
        case RelationType::MODIFIES_S:
            return *RelationshipManager::getModifiesS();
        case RelationType::MODIFIES_P:
            return *RelationshipManager::getModifiesP();
        case RelationType::USES_S:
            return *RelationshipManager::getUsesS();
        case RelationType::USES_P:
            return *RelationshipManager::getUsesP();
        default:
            return Table();
    }
}

Table DataRetriever::getTableByPattern(ExpressionSpec expressionSpec) {
    return *PatternManager::getMatchedPatterns(expressionSpec);
}

DesignEntity DataRetriever::getDesignEntityOfStmt(int stmtNumber) {
    string stmtNumberString = to_string(stmtNumber);
    string type = EntityManager::getStmtType(stmtNumberString);
    return getDesignEntity(type);
}
