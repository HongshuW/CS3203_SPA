#include "EntityManager.h"

Table EntityManager::procedureTable;
Table EntityManager::statementTable;
Table EntityManager::variableTable;
Table EntityManager::constantTable;

vector<string> EntityManager::procedureTableHeader{"$procedure_name"};
vector<string> EntityManager::statementTableHeader{"$statement_number", "$statement_type"};
vector<string> EntityManager::variableTableHeader{"$variable_name"};
vector<string> EntityManager::constantTableHeader{"$constant_value"};

void EntityManager::setHeader(Table * table, vector<string> header) {
    table->renameHeader(header);
}

Table * EntityManager::getVariables() {
    return &EntityManager::variableTable;
}

void EntityManager::saveVariables(list<string> variables) {
    // if variableTable hasn't been initialised, set header
    if (variableTable.header.size() == 0) {
        setHeader(&EntityManager::variableTable, EntityManager::variableTableHeader);
    }
    auto variablesIterator = variables.begin();
    while (variablesIterator != variables.end()) {
        vector<string> row;
        row.push_back(*variablesIterator);
        variableTable.appendRow(row);
        advance(variablesIterator, 1);
    }
}
