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
            return *RelationshipManager::getModifiesS();
        case RelationType::MODIFIES_P:
            return *RelationshipManager::getModifiesP();
        case RelationType::USES_S:
            return *RelationshipManager::getUsesS();
        case RelationType::USES_P:
            return *RelationshipManager::getUsesP();
        default:
            return Table();
    }
}
