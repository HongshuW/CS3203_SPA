//
// Created by Tan Wei En on 2/9/22.
//

#include "ProcedureNode.h"

#include <utility>


ProcedureNode::ProcedureNode(string procedureName, vector<shared_ptr<StmtNode>> stmtList) :
procedureName(std::move(std::move(procedureName))),
stmtList(std::move(stmtList)) {
}

bool ProcedureNode::operator==(const ASTNode &node) const {
    auto castedNode = dynamic_cast<const ProcedureNode*>(&node);
    bool isEqual = castedNode != nullptr && procedureName == castedNode->procedureName;
    isEqual = isEqual && equal(
            begin(stmtList), end(stmtList),
            begin(castedNode->stmtList), end(castedNode->stmtList),
            [] (const shared_ptr<StmtNode>& lhs, const shared_ptr<StmtNode>& rhs)
            {return *lhs == *rhs; });
    return isEqual;
}
