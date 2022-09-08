//
// Created by Tan Wei En on 2/9/22.
//

#ifndef SPA_ASTNODE_H
#define SPA_ASTNODE_H

#include <memory>
#include <string>

using namespace std;

namespace AST {
    class ASTNode {
    public:
        virtual ~ASTNode() = default;
        string getNodeType() {return nodeType; };
        void setNodeType(string type) {nodeType = type; };
        //TODO: override == and print method
//    virtual bool operator==(const ASTNode& node) const = 0;
//    virtual ostream & print(std::ostream & os) const = 0;
    private:
        string nodeType;
    };

}

#endif //SPA_ASTNODE_H
