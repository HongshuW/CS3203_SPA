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

void RelationshipManager::saveFollows(vector<string> follows) {
    followsTable.appendRow(follows);
}

void RelationshipManager::saveFollowsT(vector<string> followsT) {
    followsTTable.appendRow(followsT);
}

void RelationshipManager::saveParent(vector<string> parent) {
    parentTable.appendRow(parent);
}

void RelationshipManager::saveParentT(vector<string> parentT) {
    parentTTable.appendRow(parentT);
}

void RelationshipManager::saveUsesS(vector<string> usesS) {
    usesSTable.appendRow(usesS);
}

void RelationshipManager::saveUsesP(vector<string> usesP) {
    usesPTable.appendRow(usesP);
}

void RelationshipManager::saveModifiesS(vector<string> modifiesS) {
    modifiesSTable.appendRow(modifiesS);
}

void RelationshipManager::saveModifiesP(vector<string> modifiesP) {
    modifiesPTable.appendRow(modifiesP);
}
