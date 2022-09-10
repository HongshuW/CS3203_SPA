//
// Created by Aaron on 10/9/22.
//

#include "FollowsExtractor.h"
#include "AST/utils/ASTUtils.h"


void FollowsExtractor::extractFollows(shared_ptr<ProgramNode> programNode) {
    vector<shared_ptr<ProcedureNode>> procedureList = programNode->procedureList;
    for (auto procedureNode : procedureList) {
        extractFollowsFromProcedureNode(procedureNode);
    }
}

void FollowsExtractor::extractFollowsFromProcedureNode(shared_ptr<ProcedureNode> procedureNode) {
    vector<shared_ptr<StmtNode>> stmtList = procedureNode->stmtList;
    vector<shared_ptr<StmtNode>> followsList;
    for (auto stmtNode : stmtList) {
        followsList.push_back(stmtNode);
        NodeType nodeType = ASTUtils::getNodeTypeByName(stmtNode);
        switch (nodeType) {
            case AST::IF_NODE: {
                shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(stmtNode);
                vector<shared_ptr<StmtNode>> ifStmtList = ifNode->ifStmtList;
                vector<shared_ptr<StmtNode>> elseStmtList = ifNode->elseStmtList;
                extractFollowsFromStmtList(ifStmtList);
                extractFollowsFromStmtList(elseStmtList);
                break;
            }

            case AST::WHILE_NODE: {
                shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(stmtNode);
                vector<shared_ptr<StmtNode>> whileStmtList = whileNode->stmtList;
                extractFollowsFromStmtList(whileStmtList);
            }
            
            default:
                break;
        }
    }
}

void FollowsExtractor::extractFollowsFromStmtList(vector<shared_ptr<StmtNode>> stmtList) {

}

