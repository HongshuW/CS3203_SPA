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

    virtual Table getVariables();
    virtual Table getTableByDesignEntity(DesignEntity designEntity);
    virtual Table getTableByRelationType(RelationType relationType);
    virtual Table getTableByPattern(ExpressionSpec expressionSpec);
    virtual DesignEntity getDesignEntityOfStmt(int stmtNumber);

private:
    shared_ptr<PKBStorage> pkbStorage;
};

#endif //SPA_DATARETRIEVER_H
