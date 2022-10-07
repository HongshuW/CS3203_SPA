//
// Created by hongshu wang on 31/8/22.
//

#include "DataRetriever.h"

DataRetriever::DataRetriever(shared_ptr<PKBStorage> pkbStorage) {
    this->pkbStorage = pkbStorage;
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
    return *pkbStorage->getNext();
}

Table DataRetriever::getNextTTable() {
    NextTable * nextTTable = pkbStorage->getNextT();
    if (!nextTTable->isQueried()) {
        list<vector<string>> nextTRelations = cacheManager->getNextTRelations();
        nextTTable->appendRows(nextTRelations);
        nextTTable->setQueried(true);
    }
    return *nextTTable;
}

Table DataRetriever::getAffectsTable() {
    AffectsTable * affectsTable = pkbStorage->getAffects();
    if (!affectsTable->isQueried()) {
        list<vector<string>> affectsRelations = cacheManager->getAffectsRelations();
        affectsTable->appendRows(affectsRelations);
        affectsTable->setQueried(true);
    }
    return *affectsTable;
}

Table DataRetriever::getAffectsTTable() {
    AffectsTable * affectsTTable = pkbStorage->getAffectsT();
    if (!affectsTTable->isQueried()) {
        list<vector<string>> affectsTRelations = cacheManager->getAffectsTRelations();
        affectsTTable->appendRows(affectsTRelations);
        affectsTTable->setQueried(true);
    }
    return *affectsTTable;
}

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

void DataRetriever::clearCache() {
    pkbStorage->clearCache();
}
