//
// Created by hongshu wang on 31/8/22.
//

#ifndef SPA_DATARETRIEVER_H
#define SPA_DATARETRIEVER_H

#include "EntityManager.h"
#include "query_builder/commons/DesignEntity.h"
#include "query_builder/commons/RelationType.h"
#include "RelationshipManager.h"
#include "pkb/table/Table.h"


using namespace QB;

class DataRetriever {
public:
    virtual Table getVariables();
    virtual Table getTableByDesignEntity(DesignEntity designEntity);
    virtual Table getTableByRelationType(RelationType relationType);
    virtual DesignEntity getDesignEntityOfStmt(int stmtNumber);

};

#endif //SPA_DATARETRIEVER_H
