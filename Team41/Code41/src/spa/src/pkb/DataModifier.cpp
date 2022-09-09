//
// Created by hongshu wang on 31/8/22.
//

#include "DataModifier.h"
#include "EntityManager.h"
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
 * @param followedStatement a string representing the stmt_no of the followed statement
 * @param followingStatement a string representing the stmt_no of the following statement
 */
void DataModifier::saveFollows(string followedStatement, string followingStatement) {
    vector<string> follows{followedStatement, followingStatement};
    RelationshipManager::saveFollows(follows);
}

/**
 * Saves a Follows* relationship to the pkb.
 *
 * @param followedStatement a string representing the stmt_no of the followed statement
 * @param followingStatement a string representing the stmt_no of the following statement
 */
void DataModifier::saveFollowsT(string followedStatement, string followingStatement) {
    vector<string> followsT{followedStatement, followingStatement};
    RelationshipManager::saveFollowsT(followsT);
}

/**
 * Saves a Parent relationship to the pkb.
 *
 * @param parentStatement a string representing the stmt_no of the parent statement
 * @param childStatement a string representing the stmt_no of the child statement
 */
void DataModifier::saveParent(string parentStatement, string childStatement) {
    vector<string> parent{parentStatement, childStatement};
    RelationshipManager::saveParent(parent);
}

/**
 * Saves a Parent* relationship to the pkb.
 *
 * @param parentStatement a string representing the stmt_no of the parent statement
 * @param childStatement a string representing the stmt_no of the child statement
 */
void DataModifier::saveParentT(string parentStatement, string childStatement) {
    vector<string> parentT{parentStatement, childStatement};
    RelationshipManager::saveParentT(parentT);
}

/**
 * Saves a UsesS relationship to the pkb.
 *
 * @param userStatement a string representing the stmt_no of the statement in the UsesS relationship
 * @param usedEntity name of the used entity
 */
void DataModifier::saveUsesS(string userStatement, string usedEntity) {
    vector<string> usesS{userStatement, usedEntity};
    RelationshipManager::saveUsesS(usesS);
}

/**
 * Saves a UsesP relationship to the pkb.
 *
 * @param userStatement name of the entity that uses another entity in the UsesP relationship
 * @param usedEntity name of the used entity
 */
void DataModifier::saveUsesP(string userEntity, string usedEntity) {
    vector<string> usesP{userEntity, usedEntity};
    RelationshipManager::saveUsesP(usesP);
}

/**
 * Saves a ModifiesS relationship to the pkb.
 *
 * @param modifierStatement a string representing the stmt_no of the statement in the ModifiesS relationship
 * @param modifiedEntity name of the modified entity
 */
void DataModifier::saveModifiesS(string modifierStatement, string modifiedEntity) {
    vector<string> modifiesS{modifierStatement, modifiedEntity};
    RelationshipManager::saveModifiesS(modifiesS);
}

/**
 * Saves a ModifiesP relationship to the pkb.
 *
 * @param modifierStatement name of the entity that modifies another entity in the ModifiesP relationship
 * @param modifiedEntity name of the modified entity
 */
void DataModifier::saveModifiesP(string modifierEntity, string modifiedEntity) {
    vector<string> modifiesP{modifierEntity, modifiedEntity};
    RelationshipManager::saveModifiesP(modifiesP);
}
