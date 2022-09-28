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
        static void insertCallsForModifiesS(shared_ptr<ProgramNode> rootPtr,
                                            shared_ptr<list<vector<string>>> output);
        static unordered_map<string, unordered_set<string>>
            mapProceduresToModifiedVariables(shared_ptr<ProgramNode> rootPtr);
        static unordered_set<string> getModifiedVariablesFromProcedure(shared_ptr<ProcedureNode> procedureNode);
        static void handleCallNodesInModifiesP(shared_ptr<CallNode> callNode, 
            unordered_map<string, unordered_set<string>>
            mappedProceduresToModifiedVar, unordered_map<string, 
            vector<shared_ptr<CallNode>>> mappedCallNodesToProcedures,
            unordered_set<string> &uniqueVarList);
        static void handleCallNodesInModifiesS(int stmtNo, shared_ptr<CallNode> callNode, unordered_map<string, unordered_set<string>>
            mappedProceduresToModifiedVar, unordered_map<string,
            vector<shared_ptr<CallNode>>> mappedCallNodesToProcedures, shared_ptr<list<vector<string>>> output);

    public:
        static shared_ptr<list<vector<string>>> extractModifiesS(shared_ptr<ProgramNode> rootPtr);
        static shared_ptr<list<vector<string>>> extractModifiesP(shared_ptr<ProgramNode> rootPtr);
    };
}


#endif //SPA_MODIFIESEXTRACTOR_H
