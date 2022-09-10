//
// Created by Aaron on 10/9/22.
//

#include <queue>
#include "FollowsExtractor.h"
#include "AST/utils/ASTUtils.h"
#include "pkb/DataModifier.h"

using namespace std;

void FollowsExtractor::extractFollows(shared_ptr<ProgramNode> programNode) {
    vector<vector<shared_ptr<StmtNode>>> listOfStmtList = getListOfStmtList(programNode);
}

vector<vector<shared_ptr<StmtNode>>> FollowsExtractor::getListOfStmtList(shared_ptr<ProgramNode> programNode) {
    vector<vector<shared_ptr<StmtNode>>> listOfStmtList;
    vector<shared_ptr<ProcedureNode>> procedureList = programNode -> procedureList;
    shared_ptr<ProcedureNode> procedureNode = procedureList.at(0);
    queue<vector<shared_ptr<StmtNode>>> queue;
    queue.push(procedureNode->stmtList);
    while(!queue.empty()) {
        auto stmtList = queue.front();
        queue.pop();
        listOfStmtList.push_back(stmtList);
        for (auto stmtNode: stmtList) {
            NodeType nodeType = ASTUtils::getNodeType(stmtNode);
            switch (nodeType) {
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

    return listOfStmtList;
}

void FollowsExtractor::saveFollowsToPKB(vector<shared_ptr<StmtNode>> stmtList,
                                         shared_ptr<ProgramNode> programNode) {

    shared_ptr<unordered_map<shared_ptr<StmtNode>, int>>  nodeToLineNoMap =
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
        }
    }
}






