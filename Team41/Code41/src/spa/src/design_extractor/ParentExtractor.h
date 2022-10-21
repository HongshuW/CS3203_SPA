//
// Created by hongshu wang on 11/9/22.
//

#ifndef SPA_PARENTEXTRACTOR_H
#define SPA_PARENTEXTRACTOR_H

#include <map>
#include <list>
#include <vector>
#include <string>
#include "AST/ProgramNode.h"

using namespace std;

namespace DE {
    class ParentExtractor {
    private:
        static shared_ptr<map<int, vector<int>>> extractParentHashmap(shared_ptr<ProgramNode> rootPtr);
        static void extractParentTDFS(shared_ptr<map<int, vector<int>>> parentRelations, int key,
                                      vector<string> ancestors, shared_ptr<list<vector<string>>> output);

    public:
        static shared_ptr<list<vector<string>>> extractParent(shared_ptr<ProgramNode> rootPtr);
        static shared_ptr<list<vector<string>>> extractParentT(shared_ptr<ProgramNode> rootPtr);
    };
}


#endif //SPA_PARENTEXTRACTOR_H
