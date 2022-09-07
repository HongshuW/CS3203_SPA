//
// Created by hongshu wang on 31/8/22.
//

#include "DataRetriever.h"
#include "RelationshipManager.h"
#include "EntityManager.h"

Table DataRetriever::getVariables() {
    return *EntityManager::getVariables();
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
            return *RelationshipManager::getModifies();
        case RelationType::USES_S:
            return *RelationshipManager::getUses();
            //TODO: need to handle MODIFIES_P, USES_P, QPS will not return MODIFIES and USES
        default:
            return Table();
    }
}
