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

//Table DataRetriever::getTableByRelationType(RelationType relationType) {
//    switch (relationType) {
//        case RelationType::FOLLOWS:
//            return *pkbStorage->getFollows();
//        case RelationType::FOLLOWS_T:
//            return *pkbStorage->getFollowsT();
//        case RelationType::PARENT:
//            return *pkbStorage->getParent();
//        case RelationType::PARENT_T:
//            return *pkbStorage->getParentT();
//        case RelationType::MODIFIES_S:
//            return *pkbStorage->getModifiesS();
//        case RelationType::MODIFIES_P:
//            return *pkbStorage->getModifiesP();
//        case RelationType::USES_S:
//            return *pkbStorage->getUsesS();
//        case RelationType::USES_P:
//            return *pkbStorage->getUsesP();
//        case RelationType::CALLS:
//            return *pkbStorage->getCalls();
//        case RelationType::CALLS_T:
//            return *pkbStorage->getCallsT();
//        default:
//            return Table();
//    }
//}

Table DataRetriever::getFollowsTable() {
    return *pkbStorage->getFollows();
}

Table DataRetriever::getFollowsTTable() {
    return *pkbStorage->getFollowsT();
}

Table DataRetriever::getParentTable() {
    return *pkbStorage->getParent();
}

Table DataRetriever::getParentTTable() {
    return *pkbStorage->getParentT();
}

Table DataRetriever::getUsesSTable() {
    return *pkbStorage->getUsesS();
}

Table DataRetriever::getUsesPTable() {
    return *pkbStorage->getUsesP();
}

Table DataRetriever::getModifiesSTable() {
    return *pkbStorage->getModifiesS();
}

Table DataRetriever::getModifiesPTable() {
    return *pkbStorage->getModifiesP();
}

Table DataRetriever::getCallsTable() {
    return *pkbStorage->getCalls();
}

Table DataRetriever::getCallsTTable() {
    return *pkbStorage->getCallsT();
}

Table DataRetriever::getNextTable() {
    //TODO: to be implemented
    return Table();
}

Table DataRetriever::getNextTTable() {
    //TODO: to be implemented
    return Table();
}

Table DataRetriever::getAffectsTable() {
    //TODO: to be implemented
    return Table();
}

Table DataRetriever::getAffectsTTable() {
    //TODO: to be implemented
    return Table();
}

//Table DataRetriever::getTableByExprPattern(ExpressionSpec expressionSpec) {
//    return *pkbStorage->getMatchedAssignPatterns(expressionSpec);
//}
//
//Table DataRetriever::getTableByCondExprPattern(DesignEntity designEntity) {
//    switch (designEntity) {
//        case DesignEntity::WHILE:
//            return *pkbStorage->getWhilePatterns();
//        case DesignEntity::IF:
//            return *pkbStorage->getIfPatterns();
//        default:
//            return Table();
//    }
//}

Table DataRetriever::getAssignPatternTable(ExpressionSpec expressionSpec) {
    return *pkbStorage->getMatchedAssignPatterns(expressionSpec);
}

Table DataRetriever::getIfPatternTable() {
    return *pkbStorage->getIfPatterns();
}

Table DataRetriever::getWhilePatternTable() {
    return *pkbStorage->getWhilePatterns();
}

Table DataRetriever::getCallsProcedureNames() {
    return *pkbStorage->getCallsProcedureNames();
}

Table DataRetriever::getPrintVariableNames() {
    return *pkbStorage->getPrintVariableNames();
}

Table DataRetriever::getReadVariableNames() {
    return *pkbStorage->getReadVariableNames();
}

DesignEntity DataRetriever::getDesignEntityOfStmt(int stmtNumber) {
    string stmtNumberString = to_string(stmtNumber);
    string type = pkbStorage->getStmtType(stmtNumberString);
    return getDesignEntity(type);
}

unordered_set<string> DataRetriever::getFollowingStatements(int followedStatement) {
    string stmtNumberString = to_string(followedStatement);
    return *pkbStorage->getFollowingStatements(stmtNumberString);
}

unordered_set<string> DataRetriever::getChildrenStatements(int parentStatement) {
    string stmtNumberString = to_string(parentStatement);
    return *pkbStorage->getChildrenStatements(stmtNumberString);
}

unordered_set<string> DataRetriever::getModifiedVariables(int modifierStatement) {
    string stmtNumberString = to_string(modifierStatement);
    return *pkbStorage->getModifiedVariables(stmtNumberString);
}
