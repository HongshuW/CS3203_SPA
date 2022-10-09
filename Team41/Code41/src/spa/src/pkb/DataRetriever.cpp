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

shared_ptr<Table> DataRetriever::getStatementsHelper(Cachable * cachable, vector<int> metaInfo,
                                                     CacheManager::partialGetter func) {
    int stmtNo = metaInfo[0];
    int columnNumber = metaInfo[1];
    string queriedStmt = to_string(stmtNo);
    if (cachable->areAllRelationsCached()) {
        // if all relations are cached, search for the relations queried
        return cachable->getSubTableByColumn(queriedStmt, columnNumber);
    } else {
        vector<string> range = cachable->getCachedLocation(vector<string>{queriedStmt, Cachable::WILDCARD});
        if (range.empty()) {
            // if cache is empty, traverse the cfg to get relations
            shared_ptr<vector<string>> stmts = ((*cacheManager).*func)(stmtNo);
            int startIndex = cachable->rows.size();
            for (string s : *stmts) {
                cachable->appendRow(
                        columnNumber == 0 ? vector<string>{queriedStmt, s} : vector<string>{s, queriedStmt});
            }
            int endIndex = cachable->rows.size();
            cachable->setQueried(columnNumber == 0
                                 ? vector<string>{queriedStmt, Cachable::WILDCARD,
                                                  to_string(startIndex), to_string(endIndex)}
                                 : vector<string>{Cachable::WILDCARD, queriedStmt,
                                                  to_string(startIndex), to_string(endIndex)});
            return cachable->getSubTable(startIndex, endIndex);
        } else {
            // if cache is not empty, load cache
            return cachable->getSubTable(stoi(range[0]), stoi(range[1]));
        }
    }
}

void DataRetriever::getAllRelationsHelper(Cachable * cachable, CacheManager::fullGetter func) {
    if (!cachable->areAllRelationsCached()) {
        // clear cache before storing all the relations
        cachable->clearCache();
        cachable->dropRows();
        // store all the relations
        shared_ptr<list<vector<string>>> relations = ((*cacheManager).*func)();
        cachable->appendRows(*relations);
        cachable->setAllRelationsStatus(true);
    }
}

/**
 * Get all Next* Relations.
 *
 * @return all Next* Relations in a Table.
 */
Table DataRetriever::getNextTTable() {
    NextTable * nextTTable = pkbStorage->getNextT();
    getAllRelationsHelper(nextTTable, &CacheManager::getNextTRelations);
    return *nextTTable;
}

/**
 * Get all the statements s such that Next*(stmtNo, s) is true.
 *
 * @param stmtNo The statement number queried
 * @return Next* relations in a table
 */
Table DataRetriever::getNextTStatements(int stmtNo) {
    NextTable * nextTTable = pkbStorage->getNextT();
    int columnNumber = 0;
    return *getStatementsHelper(nextTTable, vector<int>{stmtNo, columnNumber},
                                &CacheManager::getNextTStatements);
}

/**
 * Get all the statements s such that Next*(s, stmtNo) is true.
 *
 * @param stmtNo The statement number queried
 * @return Next* relations in a table
 */
Table DataRetriever::getPreviousTStatements(int stmtNo) {
    NextTable * nextTTable = pkbStorage->getNextT();
    int columnNumber = 1;
    return *getStatementsHelper(nextTTable, vector<int>{stmtNo, columnNumber},
                                &CacheManager::getPreviousTStatements);
}

/**
 * Get all Affects Relations.
 *
 * @return all Affects Relations in a Table.
 */
Table DataRetriever::getAffectsTable() {
    AffectsTable * affectsTable = pkbStorage->getAffects();
    getAllRelationsHelper(affectsTable, &CacheManager::getAffectsRelations);
    return *affectsTable;
}

/**
 * Get all Affects* Relations.
 *
 * @return all Affects* Relations in a Table.
 */
Table DataRetriever::getAffectsTTable() {
    AffectsTable * affectsTTable = pkbStorage->getAffectsT();
    getAllRelationsHelper(affectsTTable, &CacheManager::getAffectsTRelations);
    return *affectsTTable;
}

/**
 * Get all statements s such that Affects(stmtNo, s) is true.
 *
 * @param stmtNo The statement number queried
 * @return Affects relations in a table
 */
Table DataRetriever::getAffectedStatements(int stmtNo) {
    AffectsTable * affectsTable = pkbStorage->getAffects();
    int columnNumber = 0;
    return *getStatementsHelper(affectsTable, vector<int>{stmtNo, columnNumber},
                                &CacheManager::getAffectedStatements);
}

/**
 * Get all statement s such that Affects(s, stmtNo) is true.
 *
 * @param stmtNo The statement number queried
 * @return Affects relations in a table
 */
Table DataRetriever::getAffectingStatements(int stmtNo) {
    AffectsTable * affectsTable = pkbStorage->getAffects();
    int columnNumber = 1;
    return *getStatementsHelper(affectsTable, vector<int>{stmtNo, columnNumber},
                                &CacheManager::getAffectingStatements);
}

/**
 * Get all statements s such that Affects*(stmtNo, s) is true.
 *
 * @param stmtNo The statement number queried
 * @return Affects* relations in a table
 */
Table DataRetriever::getAffectedTStatements(int stmtNo) {
    AffectsTable * affectsTTable = pkbStorage->getAffectsT();
    int columnNumber = 0;
    return *getStatementsHelper(affectsTTable, vector<int>{stmtNo, columnNumber},
                                &CacheManager::getAffectedTStatements);
}

/**
 * Get all statement s such that Affects*(s, stmtNo) is true.
 *
 * @param stmtNo The statement number queried
 * @return Affects* relations in a table
 */
Table DataRetriever::getAffectingTStatements(int stmtNo) {
    AffectsTable * affectsTTable = pkbStorage->getAffectsT();
    int columnNumber = 1;
    return *getStatementsHelper(affectsTTable, vector<int>{stmtNo, columnNumber},
                                &CacheManager::getAffectingTStatements);
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
