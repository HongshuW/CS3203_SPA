//
// Created by Nafour on 9/9/22.
//

#include "ASTUtils.h"

#include <queue>
#include <utility>

using namespace QB;

namespace AST {
unordered_map<string, NodeType> nodeClassNameToType({
    {typeid(AssignNode).name(), ASSIGN_NODE},
    {typeid(CallNode).name(), CALL_NODE},
    {typeid(IfNode).name(), IF_NODE},
    {typeid(PrintNode).name(), PRINT_NODE},
    {typeid(ReadNode).name(), READ_NODE},
    {typeid(WhileNode).name(), WHILE_NODE},
    {typeid(CondExprNode).name(), CONDEXPR_NODE},
    {typeid(ProgramNode).name(), PROGRAM_NODE},
    {typeid(ProcedureNode).name(), PROCEDURE_NODE},
    {typeid(VariableNode).name(), VARIABLE_NODE},
    {typeid(ExprNode).name(), EXPR_NODE},

});

unordered_map<NodeType, DesignEntity> nodeTypeToDesignEntity({
    {ASSIGN_NODE, DesignEntity::ASSIGN},
    {CALL_NODE, DesignEntity::CALL},
    {IF_NODE, DesignEntity::IF},
    {PRINT_NODE, DesignEntity::PRINT},
    {READ_NODE, DesignEntity::READ},
    {WHILE_NODE, DesignEntity::WHILE},

});

NodeType ASTUtils::getNodeType(const shared_ptr<ASTNode>& node) {
  auto& nodeObj = *node;
  string nodeType = typeid(nodeObj).name();
  return nodeClassNameToType.at(nodeType);
}

DesignEntity ASTUtils::getStmtNodeDesignEntity(
    const shared_ptr<StmtNode>& node) {
  auto& nodeObj = *node;
  string nodeTypeStr = typeid(nodeObj).name();
  NodeType nodeType = nodeClassNameToType.at(nodeTypeStr);
  return nodeTypeToDesignEntity.at(nodeType);
}

void ASTUtils::getNodeLineMapping(
    const shared_ptr<ProgramNode>& root,
    const shared_ptr<unordered_map<shared_ptr<ASTNode>, int>>& nodeToLine,
    const shared_ptr<unordered_map<int, shared_ptr<ASTNode>>>& lineToNode) {
  int curr_line_no = 1;
  for (const auto& procedureNode : root->procedureList) {
    curr_line_no = getNodeLineMappingHelper(procedureNode, nodeToLine,
                                            curr_line_no, lineToNode);
  }
}

int ASTUtils::getNodeLineMappingHelper(
    const shared_ptr<ASTNode>& node,
    const shared_ptr<unordered_map<shared_ptr<ASTNode>, int>>& nodeToLine,
    int curr_line_no,
    const shared_ptr<unordered_map<int, shared_ptr<ASTNode>>>& lineToNode) {
  NodeType nodeType = ASTUtils::getNodeType(node);

  switch (nodeType) {
    case AST::PROCEDURE_NODE: {
      shared_ptr<ProcedureNode> procedureNode =
          dynamic_pointer_cast<ProcedureNode>(node);
      for (const auto& childNode : procedureNode->stmtList) {
        curr_line_no = getNodeLineMappingHelper(childNode, nodeToLine,
                                                curr_line_no, lineToNode);
      }
      break;
    }
    case IF_NODE: {
      shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(node);
      nodeToLine->insert({ifNode, curr_line_no});
      lineToNode->insert({curr_line_no, ifNode});
      curr_line_no++;
      for (const auto& childNode : ifNode->ifStmtList) {
        curr_line_no = getNodeLineMappingHelper(childNode, nodeToLine,
                                                curr_line_no, lineToNode);
      }
      for (const auto& childNode : ifNode->elseStmtList) {
        curr_line_no = getNodeLineMappingHelper(childNode, nodeToLine,
                                                curr_line_no, lineToNode);
      }
      break;
    }
    case WHILE_NODE: {
      shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(node);
      nodeToLine->insert({whileNode, curr_line_no});
      lineToNode->insert({curr_line_no, whileNode});
      curr_line_no++;
      for (const auto& childNode : whileNode->stmtList) {
        curr_line_no = getNodeLineMappingHelper(childNode, nodeToLine,
                                                curr_line_no, lineToNode);
      }
      break;
    }
    default: {
      nodeToLine->insert({node, curr_line_no});
      lineToNode->insert({curr_line_no, node});
      curr_line_no++;
      break;
    }
  }

  return curr_line_no;
}

shared_ptr<unordered_map<shared_ptr<StmtNode>, int>>
ASTUtils::getNodePtrToLineNumMap(const shared_ptr<ProgramNode>& root) {
  shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> map =
      make_shared<unordered_map<shared_ptr<StmtNode>, int>>();
  int curr_line_no = 1;
  for (const auto& procedureNode : root->procedureList) {
    curr_line_no =
        getNodePtrToLineNoMapHelper(procedureNode, map, curr_line_no);
  }
  return map;
}

int ASTUtils::getNodePtrToLineNoMapHelper(
    const shared_ptr<ASTNode>& node,
    const shared_ptr<unordered_map<shared_ptr<StmtNode>, int>>& map,
    int curr_line_no) {
  NodeType nodeType = ASTUtils::getNodeType(node);

  switch (nodeType) {
    case AST::PROCEDURE_NODE: {
      shared_ptr<ProcedureNode> procedureNode =
          dynamic_pointer_cast<ProcedureNode>(node);
      for (const auto& childNode : procedureNode->stmtList) {
        curr_line_no =
            getNodePtrToLineNoMapHelper(childNode, map, curr_line_no);
      }
      break;
    }
    case IF_NODE: {
      shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(node);
      map->insert({ifNode, curr_line_no});
      curr_line_no++;
      for (const auto& childNode : ifNode->ifStmtList) {
        curr_line_no =
            getNodePtrToLineNoMapHelper(childNode, map, curr_line_no);
      }
      for (const auto& childNode : ifNode->elseStmtList) {
        curr_line_no =
            getNodePtrToLineNoMapHelper(childNode, map, curr_line_no);
      }
      break;
    }
    case WHILE_NODE: {
      shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(node);
      map->insert({whileNode, curr_line_no});
      curr_line_no++;
      for (const auto& childNode : whileNode->stmtList) {
        curr_line_no =
            getNodePtrToLineNoMapHelper(childNode, map, curr_line_no);
      }
      break;
    }
    default: {
      shared_ptr<StmtNode> otherStmtNode = dynamic_pointer_cast<StmtNode>(node);
      map->insert({otherStmtNode, curr_line_no});
      curr_line_no++;
      break;
    }
  }

  return curr_line_no;
}

shared_ptr<unordered_map<int, shared_ptr<StmtNode>>>
ASTUtils::getLineNumToNodePtrMap(const shared_ptr<ProgramNode>& root) {
  shared_ptr<unordered_map<int, shared_ptr<StmtNode>>> output =
      make_shared<unordered_map<int, shared_ptr<StmtNode>>>(
          unordered_map<int, shared_ptr<StmtNode>>());
  unordered_map<shared_ptr<StmtNode>, int> nodePtrToLineNumMap =
      *ASTUtils::getNodePtrToLineNumMap(root);
  for (auto& pair : nodePtrToLineNumMap) {
    shared_ptr<StmtNode> nodePtr = pair.first;
    int lineNum = pair.second;
    output->insert({lineNum, nodePtr});
  }
  return output;
}

shared_ptr<unordered_map<int, shared_ptr<ProcedureNode>>>
ASTUtils::getLineNumToProcMap(const shared_ptr<ProgramNode>& root) {
  shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> mapping =
      make_shared<unordered_map<shared_ptr<StmtNode>, int>>();
  shared_ptr<unordered_map<int, shared_ptr<ProcedureNode>>> output =
      make_shared<unordered_map<int, shared_ptr<ProcedureNode>>>();
  int curr_line_no = 1;
  for (const auto& procedureNode : root->procedureList) {
    curr_line_no =
        getNodePtrToLineNoMapHelper(procedureNode, mapping, curr_line_no);
    for (int i = 1; i < curr_line_no; i++) {
      output->insert({i, procedureNode});
    }
  }
  return output;
}

shared_ptr<unordered_map<shared_ptr<ProcedureNode>, int>>
ASTUtils::getFirstLineNumToProcMap(const shared_ptr<AST::ProgramNode>& root) {
  shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> mapping =
      make_shared<unordered_map<shared_ptr<StmtNode>, int>>();
  shared_ptr<unordered_map<shared_ptr<ProcedureNode>, int>> output =
      make_shared<unordered_map<shared_ptr<ProcedureNode>, int>>();
  int curr_line_no = 1;
  for (const auto& procedureNode : root->procedureList) {
    output->insert({procedureNode, curr_line_no});
    curr_line_no =
        getNodePtrToLineNoMapHelper(procedureNode, mapping, curr_line_no);
  }
  return output;
}

}  // namespace AST