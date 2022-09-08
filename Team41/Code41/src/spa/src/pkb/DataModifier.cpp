//
// Created by hongshu wang on 31/8/22.
//

#include "DataModifier.h"
#include "EntityManager.h"

/**
 * Saves a list of procedures to the pkb.
 *
 * @param procedures a list of names of procedures
 */
void DataModifier::saveProcedures(list<string> procedures) {
    // TODO: save procedures to pkb
}

/**
 * Saves a list of statements to the pkb.
 *
 * @param statements a list of statements, each statement is represented as a vector<string>{<stmt_no>, <stmt_type>}
 */
void DataModifier::saveStatements(list<vector<std::string>> statements) {
    // TODO: save statements to pkb
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
    // TODO: save constants to pkb
}

/**
 * Saves a Follows relationship to the pkb.
 *
 * @param followedStatement a string representing the stmt_no of the followed statement
 * @param followingStatement a string representing the stmt_no of the following statement
 */
void DataModifier::saveFollows(string followedStatement, string followingStatement) {
    // TODO: save follows relationship to pkb
}

/**
 * Saves a Follows* relationship to the pkb.
 *
 * @param followedStatement a string representing the stmt_no of the followed statement
 * @param followingStatement a string representing the stmt_no of the following statement
 */
void DataModifier::saveFollowsT(string followedStatement, string followingStatement) {
    // TODO: save follows* relationship to pkb
}

/**
 * Saves a Parent relationship to the pkb.
 *
 * @param parentStatement a string representing the stmt_no of the parent statement
 * @param childStatement a string representing the stmt_no of the child statement
 */
void DataModifier::saveParent(string parentStatement, string childStatement) {
    // TODO: save parent relationship to pkb
}

/**
 * Saves a Parent* relationship to the pkb.
 *
 * @param parentStatement a string representing the stmt_no of the parent statement
 * @param childStatement a string representing the stmt_no of the child statement
 */
void DataModifier::saveParentT(string parentStatement, string childStatement) {
    // TODO: save parent* relationship to pkb
}

/**
 * Saves a UsesS relationship to the pkb.
 *
 * @param userStatement a string representing the stmt_no of the statement in the UsesS relationship
 * @param usedEntity name of the used entity
 */
void DataModifier::saveUsesS(string userStatement, string usedEntity) {
    // TODO: save usesS relationship to pkb
}

/**
 * Saves a UsesP relationship to the pkb.
 *
 * @param userStatement name of the entity that uses another entity in the UsesP relationship
 * @param usedEntity name of the used entity
 */
void DataModifier::saveUsesP(string userEntity, string usedEntity) {
    // TODO: save usesP relationship to pkb
}

/**
 * Saves a ModifiesS relationship to the pkb.
 *
 * @param modifierStatement a string representing the stmt_no of the statement in the ModifiesS relationship
 * @param modifiedEntity name of the modified entity
 */
void DataModifier::saveModifiesS(string modifierStatement, string modifiedEntity) {
    // TODO: save modifiesS relationship to pkb
}

/**
 * Saves a ModifiesP relationship to the pkb.
 *
 * @param modifierStatement name of the entity that modifies another entity in the ModifiesP relationship
 * @param modifiedEntity name of the modified entity
 */
void DataModifier::saveModifiesP(string modifierEntity, string modifiedEntity) {
    // TODO: save modifiesP relationship to pkb
}
