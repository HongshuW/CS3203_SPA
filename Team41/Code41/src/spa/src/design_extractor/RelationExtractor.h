//
// Created by hongshu wang on 10/9/22.
//

#ifndef SPA_RELATIONEXTRACTOR_H
#define SPA_RELATIONEXTRACTOR_H

#include "AST/ProgramNode.h"
#include <list>
#include "query_builder/commons/RelationType.h"
#include <string>
#include <vector>

using namespace std;

namespace DE {
    class RelationExtractor {
    private:
        static unordered_map<int, vector<int>> extractParentHashmap(shared_ptr<ProgramNode> rootPtr);
        static shared_ptr<list<vector<string>>> extractParent(shared_ptr<ProgramNode> rootPtr);

    public:
        static list<vector<string>> extractRelation(shared_ptr<ProgramNode> rootPtr, RelationType relationType);
    };
}

#endif //SPA_RELATIONEXTRACTOR_H
