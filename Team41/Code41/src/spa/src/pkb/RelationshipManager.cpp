//
// Created by hongshu wang on 5/9/22.
//

#include "RelationshipManager.h"

Table RelationshipManager::followsTable;
Table RelationshipManager::followsTTable;
Table RelationshipManager::parentTable;
Table RelationshipManager::parentTTable;
Table RelationshipManager::usesTable;
Table RelationshipManager::modifiesTable;

vector<string> RelationshipManager::followsTableHeader{"$followed_statement", "$following_statement"};
vector<string> RelationshipManager::followsTTableHeader{"$followed_statement", "$following_statement"};
vector<string> RelationshipManager::parentTableHeader{"$parent_statement", "$child_statement"};
vector<string> RelationshipManager::parentTTableHeader{"$parent_statement", "$child_statement"};
vector<string> RelationshipManager::usesTableHeader{"$user", "$used"};
vector<string> RelationshipManager::modifiesTableHeader{"$modifier", "$modified"};

Table * RelationshipManager::getFollows() {
    return &RelationshipManager::followsTable;
}

Table * RelationshipManager::getFollowsT() {
    return &RelationshipManager::followsTTable;
}

Table * RelationshipManager::getParent() {
    return &RelationshipManager::parentTable;
}

Table * RelationshipManager::getParentT() {
    return &RelationshipManager::parentTTable;
}

Table * RelationshipManager::getUses() {
    return &RelationshipManager::usesTable;
}

Table * RelationshipManager::getModifies() {
    return &RelationshipManager::modifiesTable;
}
