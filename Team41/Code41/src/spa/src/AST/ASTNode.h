//
// Created by Tan Wei En on 2/9/22.
//

#include <string>
#include <utility>

#ifndef SPA_ASTNODE_H
#define SPA_ASTNODE_H

class ASTNode {
public:
    virtual ~ASTNode() = default;
    std::string getNodeType() { return nodeType; };
    void setNodeType(std::string type) {nodeType = std::move(type);}
private:
    std::string nodeType;
};

#endif //SPA_ASTNODE_H
