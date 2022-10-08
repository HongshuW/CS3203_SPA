//
// Created by Xingchen Lin on 8/9/22.
//

#include "IfNode.h"

#include <utility>


AST::IfNode::IfNode(shared_ptr<CondExprNode> condExpr, vector<shared_ptr<StmtNode>> ifStmtList, vector<shared_ptr<StmtNode>> elseStmtList) :
        condExpr(std::move(std::move(condExpr))), ifStmtList(std::move(ifStmtList)), elseStmtList(std::move(elseStmtList)) {}

bool IfNode::operator==(const ASTNode &node) const {
    auto castedNode = dynamic_cast<const IfNode*>(&node);
    bool isEqual = castedNode != nullptr && *condExpr == *castedNode->condExpr;
    isEqual = isEqual && equal(
            begin(ifStmtList), end(ifStmtList),
            begin(castedNode->ifStmtList), end(castedNode->ifStmtList),
            [] (const shared_ptr<StmtNode>& lhs, const shared_ptr<StmtNode>& rhs)
            {return *lhs == *rhs; });
    isEqual = isEqual && equal(
            begin(elseStmtList), end(elseStmtList),
            begin(castedNode->elseStmtList), end(castedNode->elseStmtList),
            [] (const shared_ptr<StmtNode>& lhs, const shared_ptr<StmtNode>& rhs)
            {return *lhs == *rhs; });
    return isEqual;
}