//
// Created by hongshu wang on 31/8/22.
//

#ifndef SPA_DATARETRIEVER_H
#define SPA_DATARETRIEVER_H

#include "cache_manager/CacheManager.h"
#include "query_builder/commons/DesignEntity.h"
#include "query_builder/commons/ExpressionSpec.h"
#include "query_builder/commons/RelationType.h"
#include "pkb/table/Table.h"
#include "PKBStorage.h"

using namespace QB;

class DataRetriever {
public:
    shared_ptr<CacheManager> cacheManager;

    explicit DataRetriever(shared_ptr<PKBStorage> pkbStorage);

    virtual Table getTableByDesignEntity(DesignEntity designEntity);
    virtual Table getFollowsTable();
    virtual Table getFollowsTTable();
    virtual Table getParentTable();
    virtual Table getParentTTable();
    virtual Table getUsesSTable();
    virtual Table getUsesPTable();
    virtual Table getModifiesSTable();
    virtual Table getModifiesPTable();
    virtual Table getCallsTable();
    virtual Table getCallsTTable();
    virtual Table getNextTable();
    virtual Table getNextTTable();
    virtual Table getNextTStatements(int stmtNo);
    virtual Table getPreviousTStatements(int stmtNo);
    virtual Table getNextTResult(int precedingStatement, int ensuingStatement);
    virtual Table getAffectsTable();
    virtual Table getAffectsTTable();
    virtual Table getAffectedStatements(int stmtNo);
    virtual Table getAffectingStatements(int stmtNo);
    virtual Table getAffectsResult(int affectingStatement, int affectedStatement);
    virtual Table getAffectedTStatements(int stmtNo);
    virtual Table getAffectingTStatements(int stmtNo);
    virtual Table getAffectsTResult(int affectingStatement, int affectedStatement);
    virtual Table getAssignPatternTable(ExpressionSpec expressionSpec);
    virtual Table getIfPatternTable();
    virtual Table getWhilePatternTable();
    virtual Table getCallsProcedureNames();
    virtual Table getPrintVariableNames();
    virtual Table getReadVariableNames();
    virtual DesignEntity getDesignEntityOfStmt(int stmtNumber);
    unordered_set<string> getFollowingStatements(int followedStatement);
    unordered_set<string> getChildrenStatements(int parentStatement);
    unordered_set<string> getModifiedVariables(int modifierStatement);

    void clearCache();

private:
    shared_ptr<PKBStorage> pkbStorage;

    shared_ptr<Table> getStatementsHelper(shared_ptr<Cachable> cachable, vector<int> metaInfo, CacheManager::partialGetter func);
    void getAllRelationsHelper(shared_ptr<Cachable> cachable, CacheManager::fullGetter func);
    int getDifference(vector<string> startAndEndIndices);
    shared_ptr<Table> getExactRelationHelper(shared_ptr<Cachable> cachable, vector<int> stmts, CacheManager::exactGetter func);
};

#endif //SPA_DATARETRIEVER_H
