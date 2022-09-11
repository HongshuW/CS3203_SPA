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
    public:
        static list<vector<string>> extractRelation(shared_ptr<ProgramNode> rootPtr, RelationType relationType);
    };
}

#endif //SPA_RELATIONEXTRACTOR_H
