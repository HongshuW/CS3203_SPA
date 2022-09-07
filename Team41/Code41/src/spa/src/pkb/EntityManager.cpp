#include "EntityManager.h"

string EntityManager:: STATEMENT_TABLE_COL1_NAME = "$statement_number";
string EntityManager:: STATEMENT_TABLE_COL2_NAME ="$statement_type";
string EntityManager:: VARIABLE_TABLE_COL1_NAME = "$variable_name";
string EntityManager:: CONSTANT_TABLE_COL1_NAME = "$constant_value";
string EntityManager:: USES_TABLE_COL1_NAME = "$user";
string EntityManager:: USES_TABLE_COL2_NAME = "$used";
string EntityManager:: PROCEDURE_TABLE_COL1_NAME = "$procedure_name";
string EntityManager:: PARENT_TABLE_COL1_NAME = "$parent_statement";
string EntityManager:: PARENT_TABLE_COL2_NAME = "$child_statement";
string EntityManager:: MODIFIES_TABLE_COL1_NAME = "$modifier";
string EntityManager:: MODIFIES_TABLE_COL2_NAME = "$modified";
string EntityManager:: FOLLOWS_TABLE_COL1_NAME = "$followed_statement";
string EntityManager:: FOLLOWS_TABLE_COL2_NAME = "$following_statement";

ProcedureTable EntityManager::procedureTable = ProcedureTable();
StatementTable EntityManager::statementTable = StatementTable();
VariableTable EntityManager::variableTable = VariableTable();
ConstantTable EntityManager::constantTable = ConstantTable();

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
