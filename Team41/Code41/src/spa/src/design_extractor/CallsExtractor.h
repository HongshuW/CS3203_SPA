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
private:
    static unordered_map<string, vector<shared_ptr<CallNode>>>
            getCallNodesFromProcedure(shared_ptr<ProgramNode> programNode);
};


#endif //SPA_CALLSEXTRACTOR_H
