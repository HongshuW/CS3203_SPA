//
// Created by Xingchen Lin on 8/9/22.
//

#include "WhileNode.h"

#include <utility>


AST::WhileNode::WhileNode(shared_ptr<CondExprNode> condExpr, vector<shared_ptr<StmtNode>> stmtList) :
    condExpr(std::move(std::move(condExpr))), stmtList(std::move(stmtList)) {}

bool WhileNode::operator==(const ASTNode &node) const  {
    auto castedNode = dynamic_cast<const WhileNode*>(&node);
    bool isEqual = castedNode != nullptr && *condExpr == *castedNode->condExpr;
    isEqual = isEqual && equal(
            begin(stmtList), end(stmtList),
            begin(castedNode->stmtList), end(castedNode->stmtList),
            [] (const shared_ptr<StmtNode>& lhs, const shared_ptr<StmtNode>& rhs)
            {return *lhs == *rhs; });
    return isEqual;
}
