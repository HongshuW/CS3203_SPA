//
// Created by hongshu wang on 10/9/22.
//

#ifndef SPA_RELATIONEXTRACTOR_H
#define SPA_RELATIONEXTRACTOR_H

#include "AST/ProgramNode.h"
#include <list>
#include <map>
#include "query_builder/commons/RelationType.h"
#include <set>
#include <string>
#include <vector>

using namespace std;

namespace DE {
    class RelationExtractor {
    private:
        static void extractModifiesSDFS(shared_ptr<ASTNode> node,
                                        shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNoMap,
                                        shared_ptr<vector<string>> ancestors, shared_ptr<set<vector<string>>> output);
        static shared_ptr<list<vector<string>>> extractModifiesS(shared_ptr<ProgramNode> rootPtr);

        static shared_ptr<list<vector<string>>> extractUsesS(shared_ptr<ProgramNode> rootPtr);
        static unordered_set<string> extractUsesSHelper(shared_ptr<ASTNode> node, shared_ptr<vector<vector<string>>> result, shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers);

    public:
        static list<vector<string>> extractRelation(shared_ptr<ProgramNode> rootPtr, RelationType relationType);
    };
}

#endif //SPA_RELATIONEXTRACTOR_H
