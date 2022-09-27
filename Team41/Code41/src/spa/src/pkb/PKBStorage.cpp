//
// Created by hongshu wang on 15/9/22.
//

#include "PKBStorage.h"

const string PKBStorage:: PROCEDURE_TABLE_COL1_NAME = "$procedure_name";
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
    nextTable = NextTable();
    nextTTable = NextTable();
    affectsTable = AffectsTable();
    affectsTTable = AffectsTable();
    // Pattern Tables
    assignPatternTable = PatternTable();
    whilePatternTable = PatternTable();
    ifPatternTable = PatternTable();
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

CallsTable * PKBStorage::getCalls() {
    return &callsTable;
}

CallsTable * PKBStorage::getCallsT() {
    return &callsTTable;
}

NextTable * PKBStorage::getNext() {
    return &nextTable;
}

NextTable * PKBStorage::getNextT() {
    return &nextTTable;
}

AffectsTable * PKBStorage::getAffects() {
    return &affectsTable;
}

AffectsTable * PKBStorage::getAffectsT() {
    return &affectsTTable;
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

void PKBStorage::saveCalls(vector<string> calls) {
    callsTable.appendRow(calls);
}

void PKBStorage::saveCallsT(vector<string> callsT) {
    callsTTable.appendRow(callsT);
}

void PKBStorage::saveNext(vector<string> next) {
    nextTable.appendRow(next);
}

void PKBStorage::saveNextT(vector<string> nextT) {
    nextTTable.appendRow(nextT);
}

void PKBStorage::saveAffects(vector<string> affects) {
    affectsTable.appendRow(affects);
}

void PKBStorage::saveAffectsT(vector<string> affectsT) {
    affectsTTable.appendRow(affectsT);
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
