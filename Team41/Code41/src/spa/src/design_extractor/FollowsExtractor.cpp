//
// Created by Aaron on 10/9/22.
//

#include <queue>
#include "FollowsExtractor.h"
#include "AST/utils/ASTUtils.h"
#include "pkb/DataModifier.h"


void FollowsExtractor::extractFollows(shared_ptr<ProgramNode> programNode) {
    vector<shared_ptr<ProcedureNode>> procedureList = programNode->procedureList;
    for (auto procedureNode : procedureList) {
        extractFollowsFromProcedureNode(procedureNode);
    }
}

vector<vector<shared_ptr<StmtNode>>>
        FollowsExtractor::extractFollowsFromProcedureNode(shared_ptr<ProcedureNode> procedureNode) {
    vector<vector<shared_ptr<StmtNode>>> listOfStmtList;
    queue<vector<shared_ptr<StmtNode>>> queue;
    queue.push(procedureNode->stmtList);
    while(!queue.empty()) {
        auto stmtList = queue.front();
        queue.pop();
        listOfStmtList.push_back(stmtList);
        for (auto stmtNode: stmtList) {
            NodeType nodeType = ASTUtils::getNodeTypeByName(stmtNode);
            switch (nodeType) {
                case AST::IF_NODE: {
                    shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(stmtNode);
                    vector<shared_ptr<StmtNode>> ifStmtList = ifNode->ifStmtList;
                    vector<shared_ptr<StmtNode>> elseStmtList = ifNode->elseStmtList;
                    queue.push(ifStmtList);
                    queue.push(elseStmtList);
                }

                case AST::WHILE_NODE: {
                    shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(stmtNode);
                    vector<shared_ptr<StmtNode>> whileStmtList = whileNode->stmtList;
                    queue.push(whileStmtList);
                }

                default:
                    break;
            }
        }
    }

    return listOfStmtList;
}


void FollowsExtractor::extractFollowsFromStmtList(vector<vector<shared_ptr<StmtNode>>> listOfStmtList,
                                                  shared_ptr<ProgramNode> programNode) {
    for (auto stmtList : listOfStmtList) {
        saveFollowsToPKB(stmtList, programNode);
    }
}

void FollowsExtractor::saveFollowsToPKB (vector<shared_ptr<StmtNode>> stmtList,
                                         shared_ptr<ProgramNode> programNode) {
    shared_ptr<unordered_map<shared_ptr<ASTNode>, int>> nodeToLineNoMap =
            ASTUtils::getNodePtrToLineNumMap(programNode);
    vector<int> stmtNoList;
    DataModifier dataModifier = DataModifier();
    for (auto stmtNode : stmtList) {
        int stmtNo = nodeToLineNoMap->at(stmtNode);
        stmtNoList.push_back(stmtNo);
    }
    if (stmtNoList.size() > 1) {
        std::sort(stmtNoList.begin(), stmtNoList.end());
        for (int i = 0; i < stmtNoList.size() - 1; i++) {
            string s1 = to_string(stmtNoList[i]);
            string s2 = to_string(stmtNoList[i + 1]);
            dataModifier.saveFollows(s1, s2);
            dataModifier.saveFollowsT(s1, s2);
        }
    }
}




