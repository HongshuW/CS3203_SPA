//
// Created by Aaron on 23/9/22.
//

#include "CallsExtractor.h"
#include "AST/utils/ASTUtils.h"
#include "pkb/DataModifier.h"
#include "EntityExtractor.h"
#include "queue"

shared_ptr<list<vector<string>>> CallsExtractor::extractCalls(shared_ptr<ProgramNode> programNode) {
    list<vector<string>> output;
    auto mappedCallNodesToProcedures = EntityExtractor::extractCallNodesFromProcedures(programNode);
    for (auto& it: mappedCallNodesToProcedures) {
        string name = it.first;
        vector<shared_ptr<CallNode>> listOfCallNodes = it.second;
        for(auto callNode: listOfCallNodes) {
            vector<string> callsEntry;
            string callNodeName = callNode->procedureName;
            callsEntry.push_back(name);
            callsEntry.push_back(callNodeName);
            output.push_back(callsEntry);
        }
    }
    return make_shared<list<vector<string>>>(output);
}

shared_ptr<list<vector<string>>> CallsExtractor::extractCallsStar(shared_ptr<ProgramNode> programNode) {
    list<vector<string>> output;
    auto mappedCallNodesToProcedures = EntityExtractor::extractCallNodesFromProcedures(programNode);
    for (auto& it: mappedCallNodesToProcedures) {
        string name = it.first;
        vector<shared_ptr<CallNode>> listOfCallNodes = it.second;
        unordered_set<string> reachableCallNodeNames;
        for(auto callNode: listOfCallNodes) {
            queue <shared_ptr<CallNode>> queue;
            queue.push(callNode);
            while(!queue.empty()) {
                auto callNodeEntry = queue.front();
                queue.pop();
                reachableCallNodeNames.insert(callNodeEntry->procedureName);
                if(mappedCallNodesToProcedures.count(callNodeEntry->procedureName) != 0) {
                   auto otherCallNodes = mappedCallNodesToProcedures.at(callNodeEntry-> procedureName);
                   for (auto n: otherCallNodes) {
                       queue.push(n);
                   }
                }
            }
        }
        for (auto nodeNames: reachableCallNodeNames) {
            vector<string> callsEntry;
            callsEntry.push_back(name);
            callsEntry.push_back(nodeNames);
            output.push_back(callsEntry);
        }
    }
    return make_shared<list<vector<string>>>(output);
}
