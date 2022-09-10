//
// Created by Tan Wei En on 3/9/22.
//

#ifndef SPA_DESIGNEXTRACTOR_H
#define SPA_DESIGNEXTRACTOR_H

#include <list>
#include "AST/ProgramNode.h"
#include <unordered_map>

namespace DE {
    class DesignExtractor {
    private:
        shared_ptr<ASTNode> programNode;


    public:
        DesignExtractor(shared_ptr<ASTNode> programNode);

        std::list<string> extractVariables();
        void saveVariableToPKB();

        void saveParentRelationsToPKB();
    };

}

#endif //SPA_DESIGNEXTRACTOR_H
