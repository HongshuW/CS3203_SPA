//
// Created by hongshu wang on 11/9/22.
//

#ifndef SPA_MODIFIESEXTRACTOR_H
#define SPA_MODIFIESEXTRACTOR_H

#include <set>
#include <list>

namespace DE {
    class ModifiesExtractor {
    private:
        static void extractModifiesSDFS(shared_ptr<ASTNode> node,
                                        shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNoMap,
                                        shared_ptr<vector<string>> ancestors, shared_ptr<set<vector<string>>> output);
        static void insertCallsForModifiesS(shared_ptr<ProgramNode> rootPtr,
                                            shared_ptr<list<vector<string>>> output);
        static unordered_set<string> getModifiedVariablesFromProcedure(shared_ptr<ProcedureNode> procedureNode);
        static unordered_map<string, unordered_set<string>> mapIfAndWhileStmtNoToModifiedVariables(shared_ptr<ProgramNode> rootPtr);
        static void insertCallsInIfAndWhileForModifiesS(shared_ptr<ProgramNode> rootPtr, shared_ptr<list<vector<string>>> ans);


    public:
        static unordered_map<string, unordered_set<string>>
        mapProceduresToModifiedVariables(shared_ptr<ProgramNode> rootPtr);
        static shared_ptr<list<vector<string>>> extractModifiesS(shared_ptr<ProgramNode> rootPtr);
        static shared_ptr<list<vector<string>>> extractModifiesP(shared_ptr<ProgramNode> rootPtr);
    };
}


#endif //SPA_MODIFIESEXTRACTOR_H
