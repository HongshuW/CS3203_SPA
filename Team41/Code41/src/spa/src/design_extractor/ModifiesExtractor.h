//
// Created by hongshu wang on 11/9/22.
//

#ifndef SPA_MODIFIESEXTRACTOR_H
#define SPA_MODIFIESEXTRACTOR_H

#include "RelationExtractor.h"

using namespace DE;

namespace DE {
    class ModifiesExtractor {
    private:
        static void extractModifiesSDFS(shared_ptr<ASTNode> node,
                                        shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNoMap,
                                        shared_ptr<vector<string>> ancestors, shared_ptr<set<vector<string>>> output);

    public:
        static shared_ptr<list<vector<string>>> extractModifiesS(shared_ptr<ProgramNode> rootPtr);
        static shared_ptr<list<vector<string>>> extractModifiesP(shared_ptr<ProgramNode> rootPtr);
    };
}


#endif //SPA_MODIFIESEXTRACTOR_H
