//
// Created by Aaron on 23/9/22.
//

#ifndef SPA_CALLSEXTRACTOR_H
#define SPA_CALLSEXTRACTOR_H

#include "DesignExtractor.h"
#include "AST/CallNode.h"
using namespace DE;

class CallsExtractor {
public:
    static shared_ptr<list<vector<string>>> extractCalls(shared_ptr<ProgramNode> programNode);
    static shared_ptr<list<vector<string>>> extractCallsStar(shared_ptr<ProgramNode> programNode);
    static void extractCallStmtRelationshipsToOutput(int stmtNo, shared_ptr<CallNode> callNode, unordered_map<string, unordered_set<string>>
        mappedProceduresToVar,
        unordered_map<string, vector<shared_ptr<CallNode>>> mappedCallNodesToProcedures,
        shared_ptr<list<vector<string>>> output);
    static void extractCallStmtRelationshipsWithIfAndWhileToOutput(shared_ptr<ProgramNode> rootPtr,
        unordered_map<string, unordered_set<string>> mappedProceduresToVars,
        unordered_map<string, unordered_set<string>> mappedIfAndWhileToVars,
        shared_ptr<list<vector<string>>> output);
};


#endif //SPA_CALLSEXTRACTOR_H
