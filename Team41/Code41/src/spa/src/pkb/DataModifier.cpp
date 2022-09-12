//
// Created by hongshu wang on 31/8/22.
//

#include "DataModifier.h"
#include "EntityManager.h"
#include "PatternManager.h"
#include "RelationshipManager.h"

/**
 * Saves a list of procedures to the pkb.
 *
 * @param procedures a list of names of procedures
 */
void DataModifier::saveProcedures(list<string> procedures) {
    EntityManager::saveProcedures(procedures);
}

/**
 * Saves a list of statements to the pkb.
 *
 * @param statements a list of statements, each statement is represented as a vector<string>{<stmt_no>, <stmt_type>}
 */
void DataModifier::saveStatements(list<vector<std::string>> statements) {
    EntityManager::saveStatements(statements);
}

/**
 * Saves a list of variables to the pkb.
 *
 * @param variables a list of names of variables
 */
void DataModifier::saveVariables(list<string> variables) {
    EntityManager::saveVariables(variables);
}

/**
 * Saves a list of constants to the pkb.
 *
 * @param constants a list of strings representing constant values
 */
void DataModifier::saveConstants(list<std::string> constants) {
    EntityManager::saveConstants(constants);
}

/**
 * Saves a Follows relationship to the pkb.
 *
 * @param followsRelation vector<string>{followedStatement, followingStatement}
 */
void DataModifier::saveFollows(vector<string> followsRelation) {
    RelationshipManager::saveFollows(followsRelation);
}

/**
 * Saves a Follows* relationship to the pkb.
 *
 * @param followsTRelation vector<string>{followedStatement, followingStatement}
 */
void DataModifier::saveFollowsT(vector<string> followsTRelation) {
    RelationshipManager::saveFollowsT(followsTRelation);
}

/**
 * Saves a Parent relationship to the pkb.
 *
 * @param parentRelation vector<string>{parentStatement, childStatement}
 */
void DataModifier::saveParent(vector<string> parentRelation) {
    RelationshipManager::saveParent(parentRelation);
}

/**
 * Saves a Parent* relationship to the pkb.
 *
 * @param parentTRelation vector<string>{parentStatement, childStatement}
 */
void DataModifier::saveParentT(vector<string> parentTRelation) {
    RelationshipManager::saveParentT(parentTRelation);
}

/**
 * Saves a UsesS relationship to the pkb.
 *
 * @param usesSRelation vector<string>{userStatement, usedEntity}
 */
void DataModifier::saveUsesS(vector<string> usesSRelation) {
    RelationshipManager::saveUsesS(usesSRelation);
}

/**
 * Saves a UsesP relationship to the pkb.
 *
 * @param usesPRelation vector<string>{userStatement, usedEntity}
 */
void DataModifier::saveUsesP(vector<string> usesPRelation) {
    RelationshipManager::saveUsesP(usesPRelation);
}

/**
 * Saves a ModifiesS relationship to the pkb.
 *
 * @param modifiesSRelation vector<string>{modifierStatement, modifiedEntity}
 */
void DataModifier::saveModifiesS(vector<string> modifiesSRelation) {
    RelationshipManager::saveModifiesS(modifiesSRelation);
}

/**
 * Saves a ModifiesP relationship to the pkb.
 *
 * @param modifiesPRelation vector<string>{modifierStatement, modifiedEntity}
 */
void DataModifier::saveModifiesP(vector<string> modifiesPRelation) {
    RelationshipManager::saveModifiesP(modifiesPRelation);
}

/**
 * Saves a Pattern to the pkb.
 *
 * @param metainfo vector<string>{statementNumber, variableName};
 * @param pattern
 */
void DataModifier::savePattern(vector<string> metainfo, shared_ptr<ExprNode> pattern) {
    PatternManager::savePattern(metainfo, pattern);
}
