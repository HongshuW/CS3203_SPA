#include "EntityManager.h"

ProcedureTable EntityManager::procedureTable;
StatementTable EntityManager::statementTable;
VariableTable EntityManager::variableTable;
ConstantTable EntityManager::constantTable;

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

string EntityManager::getStmtType(string stmtNumber) {
    return statementTable.getStatementType(stmtNumber);
}

void EntityManager::saveProcedures(list<string> procedures) {
    procedureTable.addValues(procedures);
}

void EntityManager::saveStatements(list<vector<string>> statements) {
    statementTable.appendRows(statements);
}

void EntityManager::saveVariables(list<string> variables) {
    variableTable.addValues(variables);
}

void EntityManager::saveConstants(list<string> constants) {
    constantTable.addValues(constants);
}
