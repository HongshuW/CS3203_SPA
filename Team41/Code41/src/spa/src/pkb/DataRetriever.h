//
// Created by hongshu wang on 31/8/22.
//

#ifndef SPA_DATARETRIEVER_H
#define SPA_DATARETRIEVER_H

#include "EntityManager.h"
#include "query_builder/commons/RelationType.h"
#include "RelationshipManager.h"
#include "Table.h"


using namespace QB;

class DataRetriever {
public:
    virtual Table getVariables();
    virtual Table getTableByRelationType(RelationType relationType);

};

#endif //SPA_DATARETRIEVER_H
