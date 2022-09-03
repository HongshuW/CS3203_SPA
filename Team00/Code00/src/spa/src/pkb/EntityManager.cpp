#include <vector>

using namespace std;

#include "EntityManager.h"

Table EntityManager::variableTable;

Table * EntityManager::getVariables() {
    return &EntityManager::variableTable;
}

void EntityManager::setHeader(Table * table, vector<string> header) {
    table->renameHeader(header);
}

void EntityManager::saveVariables(list<string> variables) {
    // if variableTable hasn't been initialised, set header
    if (variableTable.header.size() == 0) {
        vector<string> header;
        header.push_back("variable name");
        setHeader(&EntityManager::variableTable, header);
    }
    auto variablesIterator = variables.begin();
    while (variablesIterator != variables.end()) {
        vector<string> row;
        row.push_back(*variablesIterator);
        variableTable.appendRow(row);
        advance(variablesIterator, 1);
    }
}
