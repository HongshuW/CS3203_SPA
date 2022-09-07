//
// Created by hongshu wang on 31/8/22.
//

#include "DataRetriever.h"

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
        case RelationType::MODIFIES:
            return *RelationshipManager::getModifies();
        case RelationType::USES:
            return *RelationshipManager::getUses();
    }
}
