//
// Created by Aaron on 10/9/22.
//

#include <queue>
#include "FollowsExtractor.h"
#include "AST/utils/ASTUtils.h"
#include "pkb/DataModifier.h"

using namespace std;

shared_ptr<list<vector<string>>> FollowsExtractor::extractFollows(shared_ptr<ProgramNode> programNode) {
    vector<vector<shared_ptr<StmtNode>>> listOfStmtList = getListOfStmtList(programNode);
    shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> nodeToLineNoMap =
            ASTUtils::getNodePtrToLineNumMap(programNode);
    list<vector<string>> output;
    for (auto stmtList : listOfStmtList) {
        saveFollowsToOutput(stmtList, &output, nodeToLineNoMap);
    }
    return make_shared<list<vector<string>>>(output);
}

shared_ptr<list<vector<string>>> FollowsExtractor::extractFollowsStar(shared_ptr<AST::ProgramNode> programNode) {
    vector<vector<shared_ptr<StmtNode>>> listOfStmtList = getListOfStmtList(programNode);
    shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> nodeToLineNoMap =
            ASTUtils::getNodePtrToLineNumMap(programNode);
    list<vector<string>> output;
    for (auto stmtList : listOfStmtList) {
        saveFollowsStarToOutput(stmtList, &output, nodeToLineNoMap);
    }
    return make_shared<list<vector<string>>>(output);
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

void FollowsExtractor::saveFollowsToOutput(vector<shared_ptr<StmtNode>> stmtList,
                                           list<vector<string>> *output,
                                           shared_ptr<unordered_map<shared_ptr<StmtNode>, int>>
                                           nodeToLineNoMap) {
        vector<int> stmtNoList;
        if (stmtList.size() <= 1 ) {
            return;
        }
        else {
            for (auto stmtNode: stmtList) {
                int stmtNo = nodeToLineNoMap->at(stmtNode);
                stmtNoList.push_back(stmtNo);
            }

            std::sort(stmtNoList.begin(), stmtNoList.end());
            for (int i = 0; i < stmtNoList.size() - 1; i++) {
                vector<string> followsEntry;
                string s1 = to_string(stmtNoList[i]);
                string s2 = to_string(stmtNoList[i + 1]);
                followsEntry.push_back(s1);
                followsEntry.push_back(s2);
                output->push_back(followsEntry);
            }
        }
}


void FollowsExtractor::saveFollowsStarToOutput(vector<shared_ptr<StmtNode>> stmtList,
                                           list<vector<string>> *output,
                                           shared_ptr<unordered_map<shared_ptr<StmtNode>, int>>
                                           nodeToLineNoMap) {
    vector<int> stmtNoList;
    if (stmtList.size() <= 1 ) {
        return;
    }
    else {
        for (auto stmtNode: stmtList) {
            int stmtNo = nodeToLineNoMap->at(stmtNode);
            stmtNoList.push_back(stmtNo);
        }

        std::sort(stmtNoList.begin(), stmtNoList.end());
        deque<int> entryDeque;
        for (int & i : stmtNoList){
            entryDeque.push_back(i);
        }

        while(!entryDeque.empty()) {
            int front = entryDeque.front();
            entryDeque.pop_front();
            for (auto &j: entryDeque) {
                vector<string> followsEntry;
                followsEntry.push_back(to_string(front));
                followsEntry.push_back(to_string(j));
                output ->push_back(followsEntry);
            }
        }
    }
}