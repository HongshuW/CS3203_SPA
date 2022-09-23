//
// Created by Aaron on 23/9/22.
//

#include "CallsExtractor.h"
#include "AST/utils/ASTUtils.h"
#include "pkb/DataModifier.h"
#include "queue"

shared_ptr<list<vector<string>>> CallsExtractor::extractCalls(shared_ptr<ProgramNode> programNode) {
    list<vector<string>> output;
    auto mappedCallNodesToProcedures = getCallNodesFromProcedure(programNode);
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

unordered_map<string, vector<shared_ptr<CallNode>>> CallsExtractor::getCallNodesFromProcedure(shared_ptr<ProgramNode> programNode) {
    auto listOfCallNodesFromProcedures = list<vector<shared_ptr<CallNode>>>();
    auto mapCallNodesToProcedures = unordered_map<string, vector<shared_ptr<CallNode>>>();
    vector<shared_ptr<ProcedureNode>> procedureList = programNode -> procedureList;
    for (auto procedureNode: procedureList) {
        string name = procedureNode->procedureName;
        auto listOfCallNodes = vector<shared_ptr<CallNode>>();
        queue<vector<shared_ptr<StmtNode>>> queue;
        queue.push(procedureNode->stmtList);
        while(!queue.empty()) {
            auto stmtList = queue.front();
            queue.pop();
            for (auto stmtNode: stmtList) {
                NodeType nodeType = ASTUtils::getNodeType(stmtNode);
                switch (nodeType) {
                    case AST::CALL_NODE: {
                        shared_ptr<CallNode> callNode = dynamic_pointer_cast<CallNode>(stmtNode);
                        listOfCallNodes.push_back(callNode);
                        break;
                    }
                    case AST::IF_NODE: {
                        shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(stmtNode);
                        vector<shared_ptr<StmtNode>> ifStmtList = ifNode->ifStmtList;
                        vector<shared_ptr<StmtNode>> elseStmtList = ifNode->elseStmtList;
                        queue.push(ifStmtList);
                        queue.push(elseStmtList);
                        break;
                    }
                    case AST::WHILE_NODE: {
                        shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(stmtNode);
                        vector<shared_ptr<StmtNode>> whileStmtList = whileNode->stmtList;
                        queue.push(whileStmtList);
                        break;
                    }
                    default:
                        break;
                }
            }
        }
        mapCallNodesToProcedures.insert(make_pair(name, listOfCallNodes));
    }

    return mapCallNodesToProcedures;
}
