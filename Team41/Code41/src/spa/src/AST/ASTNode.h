//
// Created by Tan Wei En on 2/9/22.
//

#include <string>
#include <utility>

#ifndef SPA_ASTNODE_H
#define SPA_ASTNODE_H

using namespace std;

class ASTNode {
public:
    virtual ~ASTNode() = default;
    string getNodeType();
    void setNodeType(string type);
private:
    string nodeType;
};

#endif //SPA_ASTNODE_H
