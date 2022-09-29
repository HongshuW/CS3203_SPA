#include "CallsExtractor.h"
#include "CallsExtractor.h"
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
    shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers = ASTUtils::getNodePtrToLineNumMap(programNode);
    for (auto& it: mappedCallNodesToProcedures) {
        string name = it.first;
        vector<shared_ptr<CallNode>> listOfCallNodes = it.second;
        for(auto callNode: listOfCallNodes) {
            vector<string> callsEntry;
            string callNodeName = callNode->procedureName;
            callsEntry.push_back(name);
            callsEntry.push_back(callNodeName);
            callsEntry.push_back(to_string(stmtNumbers->at(callNode)));
            output.push_back(callsEntry);
        }
    }
    return make_shared<list<vector<string>>>(output);
}

shared_ptr<list<vector<string>>> CallsExtractor::extractCallsStar(shared_ptr<ProgramNode> programNode) {
    list<vector<string>> output;
    auto mappedCallNodesToProcedures = EntityExtractor::extractCallNodesFromProcedures(programNode);
    shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers = ASTUtils::getNodePtrToLineNumMap(programNode);
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

void CallsExtractor::extractCallStmtRelationshipsToOutput(int stmtNo, shared_ptr<CallNode> callNode, 
    unordered_map<string, unordered_set<string>> mappedProceduresToVars,
    unordered_map<string, vector<shared_ptr<CallNode>>> mappedCallNodesToProcedures, 
    shared_ptr<list<vector<string>>> output) {

    queue<shared_ptr<CallNode>> queue;
    queue.push(callNode);
    while (!queue.empty()) {
        auto callNodeEntry = queue.front();
        queue.pop();
        auto varList = mappedProceduresToVars.at(callNodeEntry->procedureName);
        for (auto var : varList) {
            vector<string> relationshipCallEntry;
            relationshipCallEntry.push_back(to_string(stmtNo));
            relationshipCallEntry.push_back(var);
            output->push_back(relationshipCallEntry);
        }

        if (mappedCallNodesToProcedures.count(callNodeEntry->procedureName) != 0) {
            auto otherCallNodes = mappedCallNodesToProcedures.at(callNodeEntry->procedureName);
            for (auto n : otherCallNodes) {
                queue.push(n);
            }
        }
    }
}

void CallsExtractor::extractCallStmtRelationshipsWithIfAndWhileToOutput(shared_ptr<ProgramNode> rootPtr, 
    unordered_map<string, unordered_set<string>> mappedProceduresToVars, 
    unordered_map<string, unordered_set<string>> mappedIfAndWhileToVars, 
    shared_ptr<list<vector<string>>> output) {

    auto ifAndWhileNodeList = EntityExtractor::extractIfAndWhileNodesFromProcedures(rootPtr);
    shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers = ASTUtils::getNodePtrToLineNumMap(rootPtr);
    auto mappedCallNodesToProcedures = EntityExtractor::extractCallNodesFromProcedures(rootPtr);


    for (auto node : ifAndWhileNodeList) {
        auto uniqueVarList = unordered_set<string>();
        int stmtNo = stmtNumbers->at(node);
        queue<shared_ptr<StmtNode>> queue;
        queue.push(node);

        while (!queue.empty()) {
            auto nodeEntry = queue.front();
            queue.pop();
            NodeType nodeType = ASTUtils::getNodeType(nodeEntry);
            switch (nodeType) {
            case AST::CALL_NODE: {
                shared_ptr<CallNode> callNode = dynamic_pointer_cast<CallNode>(nodeEntry);
                auto varList = mappedProceduresToVars.at(callNode->procedureName);

                auto usedVarList = mappedIfAndWhileToVars.at(to_string(stmtNo));
                for (auto var : varList) {
                    if (usedVarList.count(var) == 0) {
                        uniqueVarList.insert(var);
                    }
                }

                if (mappedCallNodesToProcedures.count(callNode->procedureName) != 0) {
                    auto otherCallNodes = mappedCallNodesToProcedures.at(callNode->procedureName);
                    for (auto n : otherCallNodes) {
                        queue.push(n);
                    }
                }
                break;
            }

            case AST::IF_NODE: {
                shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(nodeEntry);
                for (auto n : ifNode->ifStmtList) {
                    queue.push(n);
                }
                for (auto n : ifNode->elseStmtList) {
                    queue.push(n);
                }
                break;
            }
            case AST::WHILE_NODE: {
                shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(nodeEntry);
                for (auto n : whileNode->stmtList) {
                    queue.push(n);
                }
                break;
            }

            default:
                break;
            }

        }

        for (auto var : uniqueVarList) {
            vector<string> callEntry;
            callEntry.push_back(to_string(stmtNo));
            callEntry.push_back(var);
            output->push_back(callEntry);
        }
    }

}


