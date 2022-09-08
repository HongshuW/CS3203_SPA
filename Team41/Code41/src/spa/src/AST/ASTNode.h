//
// Created by Tan Wei En on 2/9/22.
//

#ifndef SPA_ASTNODE_H
#define SPA_ASTNODE_H

#include <memory>

using namespace std;

namespace AST {
    class ASTNode {
    public:
        virtual ~ASTNode() = default;
        //TODO: override == and print method
//    virtual bool operator==(const ASTNode& node) const = 0;
//    virtual ostream & print(std::ostream & os) const = 0;
    };
}

#endif //SPA_ASTNODE_H
