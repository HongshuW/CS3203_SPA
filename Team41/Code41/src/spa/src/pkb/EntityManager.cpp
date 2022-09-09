#include "EntityManager.h"

ProcedureTable EntityManager::procedureTable;
StatementTable EntityManager::statementTable;
VariableTable EntityManager::variableTable;
ConstantTable EntityManager::constantTable;

void EntityManager::setHeader(Table * table, vector<string> header) {
    table->renameHeader(header);
}

ProcedureTable * EntityManager::getProcedures() {
    return &EntityManager::procedureTable;
}

StatementTable * EntityManager::getStatements() {
    return &EntityManager::statementTable;
}

VariableTable * EntityManager::getVariables() {
    return &EntityManager::variableTable;
}

ConstantTable * EntityManager::getConstants() {
    return &EntityManager::constantTable;
}

void EntityManager::saveVariables(list<string> variables) {
    auto variablesIterator = variables.begin();
    while (variablesIterator != variables.end()) {
        vector<string> row;
        row.push_back(*variablesIterator);
        variableTable.appendRow(row);
        advance(variablesIterator, 1);
    }
}