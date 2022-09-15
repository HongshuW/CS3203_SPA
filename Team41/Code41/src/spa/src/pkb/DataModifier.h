//
// Created by hongshu wang on 31/8/22.
//

#ifndef SPA_DATAMODIFIER_H
#define SPA_DATAMODIFIER_H

#include "AST/ExprNode.h"
#include <list>
#include <string>
#include <vector>
#include "PKBStorage.h"

using namespace std;

class DataModifier {
public:
    explicit DataModifier(shared_ptr<PKBStorage> pkbStorage);

    // entities
    void saveProcedures(list<string> procedures);
    void saveStatements(list<vector<string>> statements);
    void saveVariables(list<string> variables);
    void saveConstants(list<string> constants);

    // relations
    void saveFollows(vector<string> followsRelation);
    void saveFollowsT(vector<string> followsTRelation);
    void saveParent(vector<string> parentRelation);
    void saveParentT(vector<string> parentTRelation);
    void saveUsesS(vector<string> usesSRelation);
    void saveUsesP(vector<string> usesPRelation);
    void saveModifiesS(vector<string> modifiesSRelation);
    void saveModifiesP(vector<string> modifiesPRelation);

    // pattern
     void savePattern(vector<string> metainfo, shared_ptr<ExprNode> pattern);

private:
    shared_ptr<PKBStorage> pkbStorage;
};

#endif //SPA_DATAMODIFIER_H
