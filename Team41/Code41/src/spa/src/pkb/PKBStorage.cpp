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
    procedureTable = make_shared<ProcedureTable>(ProcedureTable());
    statementTable = make_shared<StatementTable>(StatementTable());
    variableTable = make_shared<VariableTable>(VariableTable());
    constantTable = make_shared<ConstantTable>(ConstantTable());
    // Relation Tables
    followsTable = make_shared<FollowsTable>(FollowsTable());
    followsTTable = make_shared<FollowsTable>(FollowsTable());
    parentTable = make_shared<ParentTable>(ParentTable());
    parentTTable = make_shared<ParentTable>(ParentTable());
    usesSTable = make_shared<UsesTable>(UsesTable());
    usesPTable = make_shared<UsesTable>(UsesTable());
    modifiesSTable = make_shared<ModifiesTable>(ModifiesTable());
    modifiesPTable = make_shared<ModifiesTable>(ModifiesTable());
    callsTable = make_shared<CallsTable>(CallsTable());
    callsTTable = make_shared<CallsTable>(CallsTable());
    nextTable = make_shared<NextTable>(NextTable());
    nextTTable = make_shared<NextTable>(NextTable());
    affectsTable = make_shared<AffectsTable>(AffectsTable());
    affectsTTable = make_shared<AffectsTable>(AffectsTable());
    // Pattern Tables
    assignPatternTable = make_shared<PatternTable>(PatternTable());
    whilePatternTable = make_shared<PatternTable>(PatternTable());
    ifPatternTable = make_shared<PatternTable>(PatternTable());
}

shared_ptr<Table> PKBStorage::getProcedures() {
    return procedureTable->getProcedureNames();
}

shared_ptr<Table> PKBStorage::getStatements() {
    return statementTable->getStatements();
}

shared_ptr<Table> PKBStorage::getPrintVariableNames() {
    return statementTable->getPrintedVariables();
}

shared_ptr<Table> PKBStorage::getReadVariableNames() {
    return statementTable->getReadVariables();
}

shared_ptr<VariableTable> PKBStorage::getVariables() {
    return variableTable;
}

shared_ptr<ConstantTable> PKBStorage::getConstants() {
    return constantTable;
}

string PKBStorage::getStmtType(string stmtNumber) {
    return statementTable->getStatementType(stmtNumber);
}

void PKBStorage::saveProcedures(list<string> procedures) {
    procedureTable->addValues(procedures);
}

void PKBStorage::saveCFG(string procedure, shared_ptr<unordered_map<int, unordered_set<int>>> cfg) {
    procedureTable->saveCFGofProcedure(procedure, cfg);
}

void PKBStorage::saveStatements(list<vector<string>> statements) {
    statementTable->appendRows(statements);
}

void PKBStorage::saveVariables(list<string> variables) {
    variableTable->addValues(variables);
}

void PKBStorage::saveConstants(list<string> constants) {
    constantTable->addValues(constants);
}

shared_ptr<FollowsTable> PKBStorage::getFollows() {
    return followsTable;
}

shared_ptr<FollowsTable> PKBStorage::getFollowsT() {
    return followsTTable;
}

shared_ptr<ParentTable> PKBStorage::getParent() {
    return parentTable;
}

shared_ptr<ParentTable> PKBStorage::getParentT() {
    return parentTTable;
}

shared_ptr<UsesTable> PKBStorage::getUsesS() {
    return usesSTable;
}

shared_ptr<UsesTable> PKBStorage::getUsesP() {
    return usesPTable;
}

shared_ptr<ModifiesTable> PKBStorage::getModifiesS() {
    return modifiesSTable;
}

shared_ptr<ModifiesTable> PKBStorage::getModifiesP() {
    return modifiesPTable;
}

shared_ptr<Table> PKBStorage::getCalls() {
    return callsTable->getCallsProcedures();
}

shared_ptr<CallsTable> PKBStorage::getCallsT() {
    return callsTTable;
}

shared_ptr<Table> PKBStorage::getCallsProcedureNames() {
    return callsTable->getStmtNoProcMap();
}

shared_ptr<NextTable> PKBStorage::getNext() {
    return nextTable;
}

shared_ptr<NextTable> PKBStorage::getNextT() {
    return nextTTable;
}

shared_ptr<AffectsTable> PKBStorage::getAffects() {
    return affectsTable;
}

shared_ptr<AffectsTable> PKBStorage::getAffectsT() {
    return affectsTTable;
}

shared_ptr<unordered_set<string>> PKBStorage::getFollowingStatements(string followedStatement) {
    return followsTable->getValuesByKey(followedStatement);
}

shared_ptr<unordered_set<string>> PKBStorage::getChildrenStatements(string parentStatement) {
    return parentTable->getValuesByKey(parentStatement);
}

shared_ptr<unordered_set<string>> PKBStorage::getModifiedVariables(string modifierStatement) {
    return modifiesSTable->getValuesByKey(modifierStatement);
}

void PKBStorage::saveFollows(vector<string> follows) {
    followsTable->appendRow(follows);
}

void PKBStorage::saveFollowsT(vector<string> followsT) {
    followsTTable->appendRow(followsT);
}

void PKBStorage::saveParent(vector<string> parent) {
    parentTable->appendRow(parent);
}

void PKBStorage::saveParentT(vector<string> parentT) {
    parentTTable->appendRow(parentT);
}

void PKBStorage::saveUsesS(vector<string> usesS) {
    // append row
    usesSTable->appendRow(usesS);
    // if the statement is print, save the printed variable to the statement table
    string stmtNo = usesS[0];
    string varName = usesS[1];
    string statementType = statementTable->getStatementType(stmtNo);
    string printTypeString = getDesignEntityString(DesignEntity::PRINT);
    if (statementType == printTypeString) {
        vector<string> printStmtVarPair = vector<string>{stmtNo, varName};
        statementTable->addPrintedVar(printStmtVarPair);
    }
}

void PKBStorage::saveUsesP(vector<string> usesP) {
    usesPTable->appendRow(usesP);
}

void PKBStorage::saveModifiesS(vector<string> modifiesS) {
    // append row
    modifiesSTable->appendRow(modifiesS);
    // if the statement is read, save the read variable to the statement table
    string stmtNo = modifiesS[0];
    string varName = modifiesS[1];
    string statementType = statementTable->getStatementType(stmtNo);
    string readTypeString = getDesignEntityString(DesignEntity::READ);
    if (statementType == readTypeString) {
        vector<string> readStmtVarPair = vector<string>{stmtNo, varName};
        statementTable->addReadVar(readStmtVarPair);
    }
}

void PKBStorage::saveModifiesP(vector<string> modifiesP) {
    modifiesPTable->appendRow(modifiesP);
}

void PKBStorage::saveCalls(vector<string> calls) {
    callsTable->appendRowToSubtables(calls);
}

void PKBStorage::saveCallsT(vector<string> callsT) {
    callsTTable->appendRow(callsT);
}

void PKBStorage::saveNext(vector<string> next) {
    nextTable->appendRow(next);
}

void PKBStorage::saveNextT(vector<string> nextT) {
    nextTTable->appendRow(nextT);
}

void PKBStorage::saveAffects(vector<string> affects) {
    affectsTable->appendRow(affects);
}

void PKBStorage::saveAffectsT(vector<string> affectsT) {
    affectsTTable->appendRow(affectsT);
}

shared_ptr<Table> PKBStorage::getMatchedAssignPatterns(ExpressionSpec expressionSpec) {
    return assignPatternTable->getMatchedPatterns(expressionSpec);
}

shared_ptr<PatternTable> PKBStorage::getAssignPatterns() {
    return assignPatternTable;
}

shared_ptr<PatternTable> PKBStorage::getWhilePatterns() {
    return whilePatternTable;
}

shared_ptr<PatternTable> PKBStorage::getIfPatterns() {
    return ifPatternTable;
}

void PKBStorage::saveAssignPattern(vector<string> metainfo, shared_ptr<ExprNode> pattern) {
    assignPatternTable->addPattern(metainfo, pattern);
}

void PKBStorage::saveWhilePattern(vector<string> metainfo) {
    whilePatternTable->appendRow(metainfo);
}

void PKBStorage::saveIfPattern(vector<string> metainfo) {
    ifPatternTable->appendRow(metainfo);
}

void PKBStorage::clearCache() {
    nextTTable->clearCache();
    affectsTable->clearCache();
    affectsTTable->clearCache();
}
