//
// Created by hongshu wang on 5/9/22.
//

#include "RelationshipManager.h"

FollowsTable RelationshipManager::followsTable;
FollowsTable RelationshipManager::followsTTable;
ParentTable RelationshipManager::parentTable;
ParentTable RelationshipManager::parentTTable;
UsesTable RelationshipManager::usesSTable;
UsesTable RelationshipManager::usesPTable;
ModifiesTable RelationshipManager::modifiesSTable;
ModifiesTable RelationshipManager::modifiesPTable;

FollowsTable * RelationshipManager::getFollows() {
    return &RelationshipManager::followsTable;
}

FollowsTable * RelationshipManager::getFollowsT() {
    return &RelationshipManager::followsTTable;
}

ParentTable * RelationshipManager::getParent() {
    return &RelationshipManager::parentTable;
}

ParentTable * RelationshipManager::getParentT() {
    return &RelationshipManager::parentTTable;
}

UsesTable * RelationshipManager::getUsesS() {
    return &RelationshipManager::usesSTable;
}

UsesTable * RelationshipManager::getUsesP() {
    return &RelationshipManager::usesPTable;
}

ModifiesTable * RelationshipManager::getModifiesS() {
    return &RelationshipManager::modifiesSTable;
}

ModifiesTable * RelationshipManager::getModifiesP() {
    return &RelationshipManager::modifiesPTable;
}
