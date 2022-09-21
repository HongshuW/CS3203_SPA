//
// Created by hongshu wang on 31/8/22.
//

#include "DataModifier.h"

DataModifier::DataModifier(shared_ptr<PKBStorage> pkbStorage) {
    this->pkbStorage = pkbStorage;
}

/**
 * Saves a list of procedures to the pkb.
 *
 * @param procedures a list of names of procedures
 */
void DataModifier::saveProcedures(list<string> procedures) {
    pkbStorage->saveProcedures(procedures);
}

/**
 * Saves a list of statements to the pkb.
 *
 * @param statements a list of statements, each statement is represented as a vector<string>{<stmt_no>, <stmt_type>}
 */
void DataModifier::saveStatements(list<vector<std::string>> statements) {
    pkbStorage->saveStatements(statements);
}

/**
 * Saves a list of variables to the pkb.
 *
 * @param variables a list of names of variables
 */
void DataModifier::saveVariables(list<string> variables) {
    pkbStorage->saveVariables(variables);
}

/**
 * Saves a list of constants to the pkb.
 *
 * @param constants a list of strings representing constant values
 */
void DataModifier::saveConstants(list<std::string> constants) {
    pkbStorage->saveConstants(constants);
}

/**
 * Saves a Follows relationship to the pkb.
 *
 * @param followsRelation vector<string>{followedStatement, followingStatement}
 */
void DataModifier::saveFollows(vector<string> followsRelation) {
    pkbStorage->saveFollows(followsRelation);
}

/**
 * Saves a Follows* relationship to the pkb.
 *
 * @param followsTRelation vector<string>{followedStatement, followingStatement}
 */
void DataModifier::saveFollowsT(vector<string> followsTRelation) {
    pkbStorage->saveFollowsT(followsTRelation);
}

/**
 * Saves a Parent relationship to the pkb.
 *
 * @param parentRelation vector<string>{parentStatement, childStatement}
 */
void DataModifier::saveParent(vector<string> parentRelation) {
    pkbStorage->saveParent(parentRelation);
}

/**
 * Saves a Parent* relationship to the pkb.
 *
 * @param parentTRelation vector<string>{parentStatement, childStatement}
 */
void DataModifier::saveParentT(vector<string> parentTRelation) {
    pkbStorage->saveParentT(parentTRelation);
}

/**
 * Saves a UsesS relationship to the pkb.
 *
 * @param usesSRelation vector<string>{userStatement, usedEntity}
 */
void DataModifier::saveUsesS(vector<string> usesSRelation) {
    pkbStorage->saveUsesS(usesSRelation);
}

/**
 * Saves a UsesP relationship to the pkb.
 *
 * @param usesPRelation vector<string>{userStatement, usedEntity}
 */
void DataModifier::saveUsesP(vector<string> usesPRelation) {
    pkbStorage->saveUsesP(usesPRelation);
}

/**
 * Saves a ModifiesS relationship to the pkb.
 *
 * @param modifiesSRelation vector<string>{modifierStatement, modifiedEntity}
 */
void DataModifier::saveModifiesS(vector<string> modifiesSRelation) {
    pkbStorage->saveModifiesS(modifiesSRelation);
}

/**
 * Saves a ModifiesP relationship to the pkb.
 *
 * @param modifiesPRelation vector<string>{modifierStatement, modifiedEntity}
 */
void DataModifier::saveModifiesP(vector<string> modifiesPRelation) {
    pkbStorage->saveModifiesP(modifiesPRelation);
}

/**
 * Saves a Calls relationship to the pkb.
 *
 * @param callsRelation vector<string>{callingProcedure, calledProcedure}
 */
void DataModifier::saveCalls(vector<string> callsRelation) {
    pkbStorage->saveCalls(callsRelation);
}

/**
 * Saves a Calls* relationship to the pkb.
 *
 * @param callsTRelation vector<string>{callingProcedure, calledProcedure}
 */
void DataModifier::saveCallsT(vector<string> callsTRelation) {
    pkbStorage->saveCallsT(callsTRelation);
}

/**
 * Saves a Next relationship to the pkb.
 *
 * @param nextRelation vector<string>{precedingStatement, ensuingStatement}
 */
void DataModifier::saveNext(vector<string> nextRelation) {
    pkbStorage->saveNext(nextRelation);
}

/**
 * Saves a Next* relationship to the pkb.
 *
 * @param nextTRelation vector<string>{precedingStatement, ensuingStatement}
 */
void DataModifier::saveNextT(vector<string> nextTRelation) {
    pkbStorage->saveNextT(nextTRelation);
}

/**
 * Saves an Affects relationship to the pkb.
 *
 * @param affectsRelation vector<string>{affectingStatement, affectedStatement}
 */
void DataModifier::saveAffects(vector<string> affectsRelation) {
    pkbStorage->saveAffects(affectsRelation);
}

/**
 * Saves an Affects* relationship to the pkb.
 *
 * @param affectsTRelation vector<string>{affectingStatement, affectedStatement}
 */
void DataModifier::saveAffectsT(vector<string> affectsTRelation) {
    pkbStorage->saveAffectsT(affectsTRelation);
}

/**
 * Saves a Pattern to the pkb.
 *
 * @param metainfo vector<string>{statementNumber, variableName};
 * @param pattern
 */
void DataModifier::saveAssignPattern(vector<string> metainfo, shared_ptr<ExprNode> pattern) {
    pkbStorage->saveAssignPattern(metainfo, pattern);
}

void DataModifier::saveWhilePattern(vector<string> whilePattern) {
    pkbStorage->saveWhilePattern(whilePattern);
}

void DataModifier::saveIfPattern(vector<string> ifPattern) {
    pkbStorage->saveIfPattern(ifPattern);
}
