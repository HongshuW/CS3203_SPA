//
// Created by hongshu wang on 5/9/22.
//

#include "RelationshipManager.h"

Table RelationshipManager::followsTable;
Table RelationshipManager::followsTTable;
Table RelationshipManager::parentTable;
Table RelationshipManager::parentTTable;
Table RelationshipManager::usesSTable;
Table RelationshipManager::usesPTable;
Table RelationshipManager::modifiesSTable;
Table RelationshipManager::modifiesPTable;

vector<string> RelationshipManager::followsTableHeader{"$followed_statement", "$following_statement"};
vector<string> RelationshipManager::followsTTableHeader{"$followed_statement", "$following_statement"};
vector<string> RelationshipManager::parentTableHeader{"$parent_statement", "$child_statement"};
vector<string> RelationshipManager::parentTTableHeader{"$parent_statement", "$child_statement"};
vector<string> RelationshipManager::usesSTableHeader{"$user", "$used"};
vector<string> RelationshipManager::usesPTableHeader{"$user", "$used"};
vector<string> RelationshipManager::modifiesSTableHeader{"$modifier", "$modified"};
vector<string> RelationshipManager::modifiesPTableHeader{"$modifier", "$modified"};

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

Table * RelationshipManager::getUsesS() {
    return &RelationshipManager::usesSTable;
}

Table * RelationshipManager::getUsesP() {
    return &RelationshipManager::usesPTable;
}

Table * RelationshipManager::getModifiesS() {
    return &RelationshipManager::modifiesSTable;
}

Table * RelationshipManager::getModifiesP() {
    return &RelationshipManager::modifiesPTable;
}
