//
// Created by hongshu wang on 31/8/22.
//

#include "DataRetriever.h"
#include "./query_evaluator/constants/ClauseVisitorConstants.h"

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

shared_ptr<Table> DataRetriever::getStatementsHelper(shared_ptr<Cachable> cachable, vector<int> metaInfo,
                                                     CacheManager::partialGetter func) {
    int stmtNo = metaInfo[0];
    int columnNumber = metaInfo[1];
    string queriedStmt = to_string(stmtNo);
    if (cachable->areAllRelationsCached()) {
        // if all relations are cached, search for the relations queried
        return cachable->getSubTableByColumn(queriedStmt, columnNumber);
    } else {
        vector<string> range = cachable->getCachedLocation(columnNumber == 0
                ? vector<string>{queriedStmt, Cachable::WILDCARD}
                : vector<string>{Cachable::WILDCARD, queriedStmt});
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

void DataRetriever::getAllRelationsHelper(shared_ptr<Cachable> cachable, CacheManager::fullGetter func) {
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

int DataRetriever::getDifference(vector<string> startAndEndIndices) {
    int startIndex = stoi(startAndEndIndices[0]);
    int endIndex = stoi(startAndEndIndices[1]);
    return endIndex - startIndex;
}

shared_ptr<Table> DataRetriever::getExactRelationHelper(shared_ptr<Cachable> cachable, vector<int> stmts,
                                                        CacheManager::exactGetter func) {
    int firstStmt = stmts[0];
    int secondStmt = stmts[1];
    string first = to_string(firstStmt);
    string second = to_string(secondStmt);
    vector<string> row = vector<string>{first, second};
    bool result = false;
    if (cachable->areAllRelationsCached()) {
        // if all relations are cached, search for the queried relation
        result = cachable->contains(row, 0, cachable->rows.size());
    } else {
        vector<string> firstRange = cachable->getCachedLocation(vector<string>{first, Cachable::WILDCARD});
        vector<string> secondRange = cachable->getCachedLocation(vector<string>{Cachable::WILDCARD, second});
        if (firstRange.empty() && secondRange.empty()) {
            // if the relation hasn't been queried before, call DE to find the relation
            result = ((*cacheManager).*func)(firstStmt, secondStmt);
            return result
                ? make_shared<Table>(QE::ClauseVisitorConstants::TRUE_TABLE)
                : make_shared<Table>(QE::ClauseVisitorConstants::FALSE_TABLE);
        }
        // the relation has been queried before, search in cache
        int firstDifference = firstRange.empty() ? INT_MAX : getDifference(firstRange);
        int secondDifference = secondRange.empty() ? INT_MAX : getDifference(secondRange);
        // search in the smaller range
        if (firstDifference <= secondDifference) {
            result = cachable->contains(row, stoi(firstRange[0]), stoi(firstRange[1]));
        } else {
            result = cachable->contains(row, stoi(secondRange[0]), stoi(secondRange[1]));
        }
    }
    return result
        ? make_shared<Table>(QE::ClauseVisitorConstants::TRUE_TABLE)
        : make_shared<Table>(QE::ClauseVisitorConstants::FALSE_TABLE);
}

/**
 * Get all Next* Relations.
 *
 * @return all Next* Relations in a Table.
 */
Table DataRetriever::getNextTTable() {
    shared_ptr<NextTable> nextTTable = pkbStorage->getNextT();
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
    shared_ptr<NextTable> nextTTable = pkbStorage->getNextT();
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
    shared_ptr<NextTable> nextTTable = pkbStorage->getNextT();
    int columnNumber = 1;
    return *getStatementsHelper(nextTTable, vector<int>{stmtNo, columnNumber},
                                &CacheManager::getPreviousTStatements);
}

/**
 * Check Next*(precedingStatement, ensuingStatement) and return the result in a Table.
 *
 * @param precedingStatement The preceding statement queried
 * @param ensuingStatement The ensuing statement queried
 * @return whether the Next* relation exists
 */
Table DataRetriever::getNextTResult(int precedingStatement, int ensuingStatement) {
    shared_ptr<NextTable> nextTTable = pkbStorage->getNextT();
    return *getExactRelationHelper(
            nextTTable, vector<int>{precedingStatement, ensuingStatement}, &CacheManager::getNextTResult);
}

/**
 * Get all Affects Relations.
 *
 * @return all Affects Relations in a Table.
 */
Table DataRetriever::getAffectsTable() {
    shared_ptr<AffectsTable> affectsTable = pkbStorage->getAffects();
    getAllRelationsHelper(affectsTable, &CacheManager::getAffectsRelations);
    return *affectsTable;
}

/**
 * Get all Affects* Relations.
 *
 * @return all Affects* Relations in a Table.
 */
Table DataRetriever::getAffectsTTable() {
    shared_ptr<AffectsTable> affectsTTable = pkbStorage->getAffectsT();
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
    shared_ptr<AffectsTable> affectsTable = pkbStorage->getAffects();
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
    shared_ptr<AffectsTable> affectsTable = pkbStorage->getAffects();
    int columnNumber = 1;
    return *getStatementsHelper(affectsTable, vector<int>{stmtNo, columnNumber},
                                &CacheManager::getAffectingStatements);
}

/**
 * Check Affects(affectingStatement, affectedStatement) and return the result in a Table.
 *
 * @param affectingStatement The affecting statement queried
 * @param affectedStatement The affected statement queried
 * @return whether the Affects relation exists
 */
Table DataRetriever::getAffectsResult(int affectingStatement, int affectedStatement) {
    shared_ptr<AffectsTable> affectsTable = pkbStorage->getAffects();
    return *getExactRelationHelper(
            affectsTable, vector<int>{affectingStatement, affectedStatement}, &CacheManager::getAffectsResult);
}

/**
 * Get all statements s such that Affects*(stmtNo, s) is true.
 *
 * @param stmtNo The statement number queried
 * @return Affects* relations in a table
 */
Table DataRetriever::getAffectedTStatements(int stmtNo) {
    shared_ptr<AffectsTable> affectsTTable = pkbStorage->getAffectsT();
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
    shared_ptr<AffectsTable> affectsTTable = pkbStorage->getAffectsT();
    int columnNumber = 1;
    return *getStatementsHelper(affectsTTable, vector<int>{stmtNo, columnNumber},
                                &CacheManager::getAffectingTStatements);
}

/**
 * Check Affects*(affectingStatement, affectedStatement) and return the result in a Table.
 *
 * @param affectingStatement The affecting statement queried
 * @param affectedStatement The affected statement queried
 * @return whether the Affects* relation exists
 */
Table DataRetriever::getAffectsTResult(int affectingStatement, int affectedStatement) {
    shared_ptr<AffectsTable> affectsTTable = pkbStorage->getAffectsT();
    return *getExactRelationHelper(
            affectsTTable, vector<int>{affectingStatement, affectedStatement}, &CacheManager::getAffectsTResult);
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
