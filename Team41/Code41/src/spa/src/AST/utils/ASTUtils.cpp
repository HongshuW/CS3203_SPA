//
// Created by Nafour on 9/9/22.
//

#include "ASTUtils.h"

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
    unordered_set<NodeType> stmtNodeTypes({ASSIGN_NODE, CALL_NODE, PRINT_NODE, IF_NODE, READ_NODE, WHILE_NODE});


    NodeType ASTUtils::getNodeType(shared_ptr<ASTNode> node) {
        string nodeType = typeid(*node).name();
        return nodeClassNameToType.at(nodeType);
    }

   void ASTUtils::getNodePtrToLineNumMap(shared_ptr<ProgramNode> root, shared_ptr<unordered_map<shared_ptr<ASTNode>, int>> nodeToLine, shared_ptr<unordered_map<int, shared_ptr<ASTNode>>> lineToNode) {

        int curr_line_no = 1;
        for (auto procedureNode: root->procedureList) {
            curr_line_no = getNodePtrToLineNumMapHelper(procedureNode, nodeToLine, curr_line_no, lineToNode);
        }
    }

    int ASTUtils::getNodePtrToLineNumMapHelper(shared_ptr<ASTNode> node,
                                               shared_ptr<unordered_map<shared_ptr<ASTNode>, int>> nodeToLine, int curr_line_no, shared_ptr<unordered_map<int, shared_ptr<ASTNode>>> lineToNode) {

        NodeType nodeType = ASTUtils::getNodeType(node);

        switch (nodeType) {
            case AST::PROCEDURE_NODE: {
                shared_ptr<ProcedureNode> procedureNode = dynamic_pointer_cast<ProcedureNode>(node);
                for (auto childNode: procedureNode->stmtList) {
                    curr_line_no = getNodePtrToLineNumMapHelper(childNode, nodeToLine, curr_line_no, lineToNode);
                }
                break;
            }
            case IF_NODE: {
                shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(node);
                nodeToLine->insert({ifNode, curr_line_no});
                lineToNode->insert({curr_line_no, ifNode});
                curr_line_no++;
                for (auto childNode: ifNode->ifStmtList) {
                    curr_line_no = getNodePtrToLineNumMapHelper(childNode, nodeToLine, curr_line_no, lineToNode);
                }
                for (auto childNode: ifNode->elseStmtList) {
                    curr_line_no = getNodePtrToLineNumMapHelper(childNode, nodeToLine, curr_line_no, lineToNode);
                }
                break;
            }
            case WHILE_NODE: {
                shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(node);
                nodeToLine->insert({whileNode, curr_line_no});
                lineToNode->insert({curr_line_no, whileNode});
                curr_line_no++;
                for (auto childNode: whileNode->stmtList) {
                    curr_line_no = getNodePtrToLineNumMapHelper(childNode, nodeToLine, curr_line_no, lineToNode);
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



} // AST