//
// Created by hongshu wang on 15/9/22.
//

#include "PKBStorage.h"

string PKBStorage:: STATEMENT_TABLE_COL1_NAME = "$statement_number";
string PKBStorage:: STATEMENT_TABLE_COL2_NAME ="$statement_type";
string PKBStorage:: VARIABLE_TABLE_COL1_NAME = "$variable_name";
string PKBStorage:: CONSTANT_TABLE_COL1_NAME = "$constant_value";
string PKBStorage:: USES_TABLE_COL1_NAME = "$user";
string PKBStorage:: USES_TABLE_COL2_NAME = "$used";
string PKBStorage:: PROCEDURE_TABLE_COL1_NAME = "$procedure_name";
string PKBStorage:: PARENT_TABLE_COL1_NAME = "$parent_statement";
string PKBStorage:: PARENT_TABLE_COL2_NAME = "$child_statement";
string PKBStorage:: MODIFIES_TABLE_COL1_NAME = "$modifier";
string PKBStorage:: MODIFIES_TABLE_COL2_NAME = "$modified";
string PKBStorage:: FOLLOWS_TABLE_COL1_NAME = "$followed_statement";
string PKBStorage:: FOLLOWS_TABLE_COL2_NAME = "$following_statement";

PKBStorage::PKBStorage() {
    // Entity Tables
    procedureTable = ProcedureTable();
    statementTable = StatementTable();
    variableTable = VariableTable();
    constantTable = ConstantTable();
    // Relation Tables
    followsTable = FollowsTable();
    followsTTable = FollowsTable();
    parentTable = ParentTable();
    parentTable = ParentTable();
    usesSTable = UsesTable();
    usesPTable = UsesTable();
    modifiesSTable = ModifiesTable();
    modifiesPTable = ModifiesTable();
    // Pattern Tables
    patternTable = PatternTable();
}

ProcedureTable * PKBStorage::getProcedures() {
    return &procedureTable;
}

StatementTable * PKBStorage::getStatements() {
    return &statementTable;
}

VariableTable * PKBStorage::getVariables() {
    return &variableTable;
}

ConstantTable * PKBStorage::getConstants() {
    return &constantTable;
}

string PKBStorage::getStmtType(string stmtNumber) {
    return statementTable.getStatementType(stmtNumber);
}

void PKBStorage::saveProcedures(list<string> procedures) {
    procedureTable.addValues(procedures);
}

void PKBStorage::saveStatements(list<vector<string>> statements) {
    statementTable.appendRows(statements);
}

void PKBStorage::saveVariables(list<string> variables) {
    variableTable.addValues(variables);
}

void PKBStorage::saveConstants(list<string> constants) {
    constantTable.addValues(constants);
}

FollowsTable * PKBStorage::getFollows() {
    return &followsTable;
}

FollowsTable * PKBStorage::getFollowsT() {
    return &followsTTable;
}

ParentTable * PKBStorage::getParent() {
    return &parentTable;
}

ParentTable * PKBStorage::getParentT() {
    return &parentTTable;
}

UsesTable * PKBStorage::getUsesS() {
    return &usesSTable;
}

UsesTable * PKBStorage::getUsesP() {
    return &usesPTable;
}

ModifiesTable * PKBStorage::getModifiesS() {
    return &modifiesSTable;
}

ModifiesTable * PKBStorage::getModifiesP() {
    return &modifiesPTable;
}

void PKBStorage::saveFollows(vector<string> follows) {
    followsTable.appendRow(follows);
}

void PKBStorage::saveFollowsT(vector<string> followsT) {
    followsTTable.appendRow(followsT);
}

void PKBStorage::saveParent(vector<string> parent) {
    parentTable.appendRow(parent);
}

void PKBStorage::saveParentT(vector<string> parentT) {
    parentTTable.appendRow(parentT);
}

void PKBStorage::saveUsesS(vector<string> usesS) {
    usesSTable.appendRow(usesS);
}

void PKBStorage::saveUsesP(vector<string> usesP) {
    usesPTable.appendRow(usesP);
}

void PKBStorage::saveModifiesS(vector<string> modifiesS) {
    modifiesSTable.appendRow(modifiesS);
}

void PKBStorage::saveModifiesP(vector<string> modifiesP) {
    modifiesPTable.appendRow(modifiesP);
}

shared_ptr<Table> PKBStorage::getMatchedPatterns(ExpressionSpec expressionSpec) {
    return patternTable.getMatchedPatterns(expressionSpec);
}

PatternTable * PKBStorage::getPatterns() {
    return &patternTable;
}

void PKBStorage::savePattern(vector<string> metainfo, shared_ptr<ExprNode> pattern) {
    patternTable.addPattern(metainfo, pattern);
}
