//
// Created by Nafour on 11/9/22.
//

#ifndef SPA_PATTERNEXTRACTOR_H
#define SPA_PATTERNEXTRACTOR_H

#include <string>
#include <vector>
#include "AST/AssignNode.h"
#include "AST/ProgramNode.h"
#include <unordered_map>
namespace DE {

    class PatternExtractor {
        static void extractPatternHelper(shared_ptr<ASTNode> node, shared_ptr<vector<pair<pair<int, string>, std::shared_ptr<AssignNode>>>> resultVec, shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers);
    public:
        static vector<pair<pair<int, string>, std::shared_ptr<AssignNode>>> extractPattern(shared_ptr<ProgramNode> programNode);
    };

} // DE

#endif //SPA_PATTERNEXTRACTOR_H
