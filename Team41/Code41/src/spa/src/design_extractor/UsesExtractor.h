//
// Created by Nafour on 11/9/22.
//

#ifndef SPA_USESEXTRACTOR_H
#define SPA_USESEXTRACTOR_H

#include <string>
#include "AST/ProgramNode.h"
#include "unordered_set"
#include "list"
#include "unordered_map"

namespace DE {

    class UsesExtractor {
    public:
        static shared_ptr<list<vector<string>>> extractUsesS(shared_ptr<ProgramNode> rootPtr);
    private:
        static unordered_set<string> extractUsesSHelper(shared_ptr<ASTNode> node, shared_ptr<vector<vector<string>>> result, shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers);
    };

} // DE

#endif //SPA_USESEXTRACTOR_H
