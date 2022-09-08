//
// Created by Tan Wei En on 8/9/22.
//

#include <vector>
#include "ASTNode.h"

#ifndef SPA_STATEMENTLISTNODE_H
#define SPA_STATEMENTLISTNODE_H

using namespace std;

class StatementListNode : public ASTNode {
private:
    vector<shared_ptr<ASTNode>> statementList;
public:
    StatementListNode(vector<shared_ptr<ASTNode>> statementList);
    vector<shared_ptr<ASTNode>> getStatementList();
};

#endif //SPA_STATEMENTLISTNODE_H