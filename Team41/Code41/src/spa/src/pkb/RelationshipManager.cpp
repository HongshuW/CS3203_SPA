//
// Created by hongshu wang on 5/9/22.
//

#include "RelationshipManager.h"

Table RelationshipManager::followsTable;
Table RelationshipManager::followsStarTable;
Table RelationshipManager::parentTable;
Table RelationshipManager::parentStarTable;
Table RelationshipManager::usesTable;
Table RelationshipManager::modifiesTable;

vector<string> RelationshipManager::followsTableHeader{"$followed_statement", "$following_statement"};
vector<string> RelationshipManager::followsStarTableHeader{"$followed_statement", "$following_statement"};
vector<string> RelationshipManager::parentTableHeader{"$parent_statement", "$child_statement"};
vector<string> RelationshipManager::parentStarTableHeader{"$parent_statement", "$child_statement"};
vector<string> RelationshipManager::usesTableHeader{"$user", "$used"};
vector<string> RelationshipManager::modifiesTableHeader{"$modifier", "$modified"};

Table * RelationshipManager::getFollows() {
    return &RelationshipManager::followsTable;
}

Table * RelationshipManager::getFollowsStar() {
    return &RelationshipManager::followsStarTable;
}

Table * RelationshipManager::getParent() {
    return &RelationshipManager::parentTable;
}

Table * RelationshipManager::getParentStar() {
    return &RelationshipManager::parentStarTable;
}

Table * RelationshipManager::getUses() {
    return &RelationshipManager::usesTable;
}

Table * RelationshipManager::getModifies() {
    return &RelationshipManager::modifiesTable;
}
