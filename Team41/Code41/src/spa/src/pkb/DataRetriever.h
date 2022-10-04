//
// Created by hongshu wang on 31/8/22.
//

#ifndef SPA_DATARETRIEVER_H
#define SPA_DATARETRIEVER_H

#include "query_builder/commons/DesignEntity.h"
#include "query_builder/commons/ExpressionSpec.h"
#include "query_builder/commons/RelationType.h"
#include "pkb/table/Table.h"
#include "PKBStorage.h"

using namespace QB;

class DataRetriever {
public:
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
    virtual Table getAffectsTable();
    virtual Table getAffectsTTable();
    virtual Table getAssignPatternTable(ExpressionSpec expressionSpec);
    virtual Table getIfPatternTable();
    virtual Table getWhilePatternTable();
    Table getCallsProcedureNames();
    Table getPrintVariableNames();
    Table getReadVariableNames();
    virtual DesignEntity getDesignEntityOfStmt(int stmtNumber);
    unordered_set<string> getFollowingStatements(int followedStatement);
    unordered_set<string> getChildrenStatements(int parentStatement);
    unordered_set<string> getModifiedVariables(int modifierStatement);

    void clearCache();

private:
    shared_ptr<PKBStorage> pkbStorage;
};

#endif //SPA_DATARETRIEVER_H
