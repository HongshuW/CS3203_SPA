//
// Created by hongshu wang on 10/9/22.
//

#ifndef SPA_RELATIONEXTRACTOR_H
#define SPA_RELATIONEXTRACTOR_H

#include "AST/ProgramNode.h"
#include <list>
#include <map>
#include "query_builder/commons/RelationType.h"
#include <string>
#include <vector>

using namespace std;

namespace DE {
    class RelationExtractor {
    private:
        static shared_ptr<map<int, vector<int>>> extractParentHashmap(shared_ptr<ProgramNode> rootPtr);
        static shared_ptr<list<vector<string>>> extractParent(shared_ptr<ProgramNode> rootPtr);
        static void extractParentTDFS(shared_ptr<map<int, vector<int>>> parentRelations, int key,
                                      shared_ptr<vector<string>> ancestors, shared_ptr<list<vector<string>>> output);
        static shared_ptr<list<vector<string>>> extractParentT(shared_ptr<ProgramNode> rootPtr);

        static shared_ptr<list<vector<string>>> extractUsesS(shared_ptr<ProgramNode> rootPtr);
        static unordered_set<string> extractUsesSHelper(shared_ptr<ASTNode> node, shared_ptr<vector<vector<string>>> result, shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers);

    public:
        static list<vector<string>> extractRelation(shared_ptr<ProgramNode> rootPtr, RelationType relationType);
    };
}

#endif //SPA_RELATIONEXTRACTOR_H
