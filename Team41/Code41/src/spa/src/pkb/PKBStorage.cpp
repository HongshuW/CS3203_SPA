//
// Created by hongshu wang on 15/9/22.
//

#include "PKBStorage.h"
#include "query_builder/commons/DesignEntity.h"

const string PKBStorage:: PROCEDURE_TABLE_COL1_NAME = "$procedure_name";
const string PKBStorage:: PROCEDURE_TABLE_COL2_NAME = "$cfg_ptr";
const string PKBStorage:: STATEMENT_TABLE_COL1_NAME = "$statement_number";
const string PKBStorage:: STATEMENT_TABLE_COL2_NAME ="$statement_type";
const string PKBStorage:: VARIABLE_TABLE_COL1_NAME = "$variable_name";
const string PKBStorage:: CONSTANT_TABLE_COL1_NAME = "$constant_value";
const string PKBStorage:: USES_TABLE_COL1_NAME = "$user";
const string PKBStorage:: USES_TABLE_COL2_NAME = "$used";
const string PKBStorage:: PARENT_TABLE_COL1_NAME = "$parent_statement";
const string PKBStorage:: PARENT_TABLE_COL2_NAME = "$child_statement";
const string PKBStorage:: MODIFIES_TABLE_COL1_NAME = "$modifier";
const string PKBStorage:: MODIFIES_TABLE_COL2_NAME = "$modified";
const string PKBStorage:: FOLLOWS_TABLE_COL1_NAME = "$followed_statement";
const string PKBStorage:: FOLLOWS_TABLE_COL2_NAME = "$following_statement";
const string PKBStorage:: CALLS_TABLE_COL1_NAME = "$calling_procedure";
const string PKBStorage:: CALLS_TABLE_COL2_NAME = "$called_procedure";
const string PKBStorage:: NEXT_TABLE_COL1_NAME = "$preceding_statement";
const string PKBStorage:: NEXT_TABLE_COL2_NAME = "$ensuing_statement";
const string PKBStorage:: AFFECTS_TABLE_COL1_NAME = "$affecting_statement";
const string PKBStorage:: AFFECTS_TABLE_COL2_NAME = "$affected_statement";

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
    parentTTable = ParentTable();
    usesSTable = UsesTable();
    usesPTable = UsesTable();
    modifiesSTable = ModifiesTable();
    modifiesPTable = ModifiesTable();
    callsTable = CallsTable();
    callsTTable = CallsTable();
    // Pattern Tables
    assignPatternTable = PatternTable();
    whilePatternTable = PatternTable();
    ifPatternTable = PatternTable();
}

shared_ptr<Table> PKBStorage::getProcedures() {
    return procedureTable.getProcedureNames();
}

shared_ptr<Table> PKBStorage::getStatements() {
    return statementTable.getStatements();
}

shared_ptr<Table> PKBStorage::getPrintVariableNames() {
    return statementTable.getPrintedVariables();
}

shared_ptr<Table> PKBStorage::getReadVariableNames() {
    return statementTable.getReadVariables();
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

void PKBStorage::saveCFG(string procedure, shared_ptr<unordered_map<int, unordered_set<int>>> cfg) {
    procedureTable.saveCFGofProcedure(procedure, cfg);
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

shared_ptr<Table> PKBStorage::getCalls() {
    return callsTable.getCallsProcedures();
}

CallsTable * PKBStorage::getCallsT() {
    return &callsTTable;
}

shared_ptr<Table> PKBStorage::getCallsProcedureNames() {
    return callsTable.getStmtNoProcMap();
}

shared_ptr<unordered_set<string>> PKBStorage::getFollowingStatements(string followedStatement) {
    return followsTable.getValuesByKey(followedStatement);
}

shared_ptr<unordered_set<string>> PKBStorage::getChildrenStatements(string parentStatement) {
    return parentTable.getValuesByKey(parentStatement);
}

shared_ptr<unordered_set<string>> PKBStorage::getModifiedVariables(string modifierStatement) {
    return modifiesSTable.getValuesByKey(modifierStatement);
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
    // append row
    usesSTable.appendRow(usesS);
    // if the statement is print, save the printed variable to the statement table
    string stmtNo = usesS[0];
    string varName = usesS[1];
    string statementType = statementTable.getStatementType(stmtNo);
    string printTypeString = getDesignEntityString(DesignEntity::PRINT);
    if (statementType == printTypeString) {
        vector<string> printStmtVarPair = vector<string>{stmtNo, varName};
        statementTable.addPrintedVar(printStmtVarPair);
    }
}

void PKBStorage::saveUsesP(vector<string> usesP) {
    usesPTable.appendRow(usesP);
}

void PKBStorage::saveModifiesS(vector<string> modifiesS) {
    // append row
    modifiesSTable.appendRow(modifiesS);
    // if the statement is read, save the read variable to the statement table
    string stmtNo = modifiesS[0];
    string varName = modifiesS[1];
    string statementType = statementTable.getStatementType(stmtNo);
    string readTypeString = getDesignEntityString(DesignEntity::READ);
    if (statementType == readTypeString) {
        vector<string> readStmtVarPair = vector<string>{stmtNo, varName};
        statementTable.addReadVar(readStmtVarPair);
    }
}

void PKBStorage::saveModifiesP(vector<string> modifiesP) {
    modifiesPTable.appendRow(modifiesP);
}

void PKBStorage::saveCalls(vector<string> calls) {
    callsTable.appendRowToSubtables(calls);
}

void PKBStorage::saveCallsT(vector<string> callsT) {
    callsTTable.appendRow(callsT);
}

shared_ptr<Table> PKBStorage::getMatchedAssignPatterns(ExpressionSpec expressionSpec) {
    return assignPatternTable.getMatchedPatterns(expressionSpec);
}

PatternTable * PKBStorage::getAssignPatterns() {
    return &assignPatternTable;
}

PatternTable * PKBStorage::getWhilePatterns() {
    return &whilePatternTable;
}

PatternTable * PKBStorage::getIfPatterns() {
    return &ifPatternTable;
}

void PKBStorage::saveAssignPattern(vector<string> metainfo, shared_ptr<ExprNode> pattern) {
    assignPatternTable.addPattern(metainfo, pattern);
}

void PKBStorage::saveWhilePattern(vector<string> metainfo) {
    whilePatternTable.appendRow(metainfo);
}

void PKBStorage::saveIfPattern(vector<string> metainfo) {
    ifPatternTable.appendRow(metainfo);
}
