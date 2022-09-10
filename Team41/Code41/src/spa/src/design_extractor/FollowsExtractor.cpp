//
// Created by Aaron on 10/9/22.
//

#include <queue>
#include "FollowsExtractor.h"
#include "AST/utils/ASTUtils.h"


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
        listOfStmtList.push_back(stmtList);
        for(auto stmtNode: stmtList) {
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



//void FollowsExtractor::extractFollowsFromProcedureNode(shared_ptr<ProcedureNode> procedureNode) {
//    vector<shared_ptr<StmtNode>> stmtList = procedureNode->stmtList;
//    vector<shared_ptr<StmtNode>> followsList;
//    for (auto stmtNode : stmtList) {
//        followsList.push_back(stmtNode);
//        NodeType nodeType = ASTUtils::getNodeTypeByName(stmtNode);
//        switch (nodeType) {
//            case AST::IF_NODE: {
//                shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(stmtNode);
//                vector<shared_ptr<StmtNode>> ifStmtList = ifNode->ifStmtList;
//                vector<shared_ptr<StmtNode>> elseStmtList = ifNode->elseStmtList;
//                extractFollowsFromStmtList(ifStmtList);
//                extractFollowsFromStmtList(elseStmtList);
//                break;
//            }
//
//            case AST::WHILE_NODE: {
//                shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(stmtNode);
//                vector<shared_ptr<StmtNode>> whileStmtList = whileNode->stmtList;
//                extractFollowsFromStmtList(whileStmtList);
//            }
//
//            default:
//                break;
//        }
//    }
//    listOfFollowLists.push_back(followsList);
//}
//
//void FollowsExtractor::extractFollowsFromStmtList(vector<shared_ptr<StmtNode>> stmtList) {
//    vector<shared_ptr<StmtNode>> followsList;
//    for (auto stmtNode : stmtList) {
//        followsList.push_back(stmtNode);
//        NodeType nodeType = ASTUtils::getNodeTypeByName(stmtNode);
//        switch (nodeType) {
//            case AST::IF_NODE: {
//                shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(stmtNode);
//                vector<shared_ptr<StmtNode>> ifStmtList = ifNode->ifStmtList;
//                vector<shared_ptr<StmtNode>> elseStmtList = ifNode->elseStmtList;
//                extractFollowsFromStmtList(ifStmtList);
//                extractFollowsFromStmtList(elseStmtList);
//                break;
//            }
//
//            case AST::WHILE_NODE: {
//                shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(stmtNode);
//                vector<shared_ptr<StmtNode>> whileStmtList = whileNode->stmtList;
//                extractFollowsFromStmtList(whileStmtList);
//            }
//
//            default:
//                break;
//        }
//    }
//    listOfFollowLists.push_back(followsList);
//}
//
//void FollowsExtractor::saveFollowsToPKB(vector<shared_ptr<StmtNode>> followsList) {
//    for (auto stmtNode : followsList) {
//        ASTUtils::getNodePtrToLineNumMap();
//    }
//}

