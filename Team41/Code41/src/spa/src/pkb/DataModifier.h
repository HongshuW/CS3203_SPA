//
// Created by hongshu wang on 31/8/22.
//

#ifndef SPA_DATAMODIFIER_H
#define SPA_DATAMODIFIER_H

#include <list>
#include <string>

using namespace std;

class DataModifier {
public:
    // entities
    void saveProcedures(list<string> procedures);
    void saveStatements(list<vector<string>> statements);
    void saveVariables(list<string> variables);
    void saveConstants(list<string> constants);

    // relations
    void saveFollows(string followedStatement, string followingStatement);
    void saveFollowsT(string followedStatement, string followingStatement);
    void saveParent(string parentStatement, string childStatement);
    void saveParentT(string parentStatement, string childStatement);
    void saveUsesS(string userStatement, string usedEntity);
    void saveUsesP(string userEntity, string usedEntity);
    void saveModifiesS(string modifierStatement, string modifiedEntity);
    void saveModifiesP(string modifierEntity, string modifiedEntity);

    // pattern
//    void savePattern(AST_NODE pattern);
};

#endif //SPA_DATAMODIFIER_H
