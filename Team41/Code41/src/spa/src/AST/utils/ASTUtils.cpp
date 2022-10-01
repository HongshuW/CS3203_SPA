//
// Created by Nafour on 9/9/22.
//

#include "ASTUtils.h"

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
    unordered_set<NodeType> stmtNodeTypes({ASSIGN_NODE, CALL_NODE, PRINT_NODE, IF_NODE, READ_NODE, WHILE_NODE});


    NodeType ASTUtils::getNodeType(shared_ptr<ASTNode> node) {
        string nodeType = typeid(*node).name();
        return nodeClassNameToType.at(nodeType);
    }
    DesignEntity ASTUtils::getStmtNodeDesignEntity(shared_ptr<StmtNode> node) {
        string nodeTypeStr = typeid(*node).name();
        NodeType nodeType = nodeClassNameToType.at(nodeTypeStr);
        return nodeTypeToDesignEntity.at(nodeType);
    }

    shared_ptr<unordered_map<int, shared_ptr<ProcedureNode>>> ASTUtils::getLineNumToProcMap(shared_ptr<ProgramNode> root) {
        shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> map = 
            make_shared<unordered_map<shared_ptr<StmtNode>, int>>();
        shared_ptr<unordered_map<int, shared_ptr<ProcedureNode>>> output = 
            make_shared<unordered_map<int, shared_ptr<ProcedureNode>>>();
        int curr_line_no = 1;
        for (auto procedureNode : root->procedureList) {
            curr_line_no = getNodePtrToLineNoMapHelper(procedureNode, map, curr_line_no);
            for (int i = 1; i < curr_line_no; i++) {
                output->insert({ i, procedureNode });
            }
        }
        return output;
    }

   void ASTUtils::getNodeLineMapping(shared_ptr<ProgramNode> root, shared_ptr<unordered_map<shared_ptr<ASTNode>, int>> nodeToLine, shared_ptr<unordered_map<int, shared_ptr<ASTNode>>> lineToNode) {

        int curr_line_no = 1;
        for (auto procedureNode: root->procedureList) {
            curr_line_no = getNodeLineMappingHelper(procedureNode, nodeToLine, curr_line_no, lineToNode);
        }
    }

    int ASTUtils::getNodeLineMappingHelper(shared_ptr<ASTNode> node,
                                           shared_ptr<unordered_map<shared_ptr<ASTNode>, int>> nodeToLine, int curr_line_no, shared_ptr<unordered_map<int, shared_ptr<ASTNode>>> lineToNode) {

        NodeType nodeType = ASTUtils::getNodeType(node);

        switch (nodeType) {
            case AST::PROCEDURE_NODE: {
                shared_ptr<ProcedureNode> procedureNode = dynamic_pointer_cast<ProcedureNode>(node);
                for (auto childNode: procedureNode->stmtList) {
                    curr_line_no = getNodeLineMappingHelper(childNode, nodeToLine, curr_line_no, lineToNode);
                }
                break;
            }
            case IF_NODE: {
                shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(node);
                nodeToLine->insert({ifNode, curr_line_no});
                lineToNode->insert({curr_line_no, ifNode});
                curr_line_no++;
                for (auto childNode: ifNode->ifStmtList) {
                    curr_line_no = getNodeLineMappingHelper(childNode, nodeToLine, curr_line_no, lineToNode);
                }
                for (auto childNode: ifNode->elseStmtList) {
                    curr_line_no = getNodeLineMappingHelper(childNode, nodeToLine, curr_line_no, lineToNode);
                }
                break;
            }
            case WHILE_NODE: {
                shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(node);
                nodeToLine->insert({whileNode, curr_line_no});
                lineToNode->insert({curr_line_no, whileNode});
                curr_line_no++;
                for (auto childNode: whileNode->stmtList) {
                    curr_line_no = getNodeLineMappingHelper(childNode, nodeToLine, curr_line_no, lineToNode);
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

    shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> ASTUtils::getNodePtrToLineNumMap(shared_ptr<ProgramNode> root) {
        shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> map = make_shared<unordered_map<shared_ptr<StmtNode>, int>>();
        int curr_line_no = 1;
        for (auto procedureNode: root->procedureList) {
            curr_line_no = getNodePtrToLineNoMapHelper(procedureNode, map, curr_line_no);
        }

        return map;
    }

    int ASTUtils::getNodePtrToLineNoMapHelper(shared_ptr<ASTNode> node,
                                              shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> map, int curr_line_no) {

        NodeType nodeType = ASTUtils::getNodeType(node);

        switch (nodeType) {
            case AST::PROCEDURE_NODE: {
                shared_ptr<ProcedureNode> procedureNode = dynamic_pointer_cast<ProcedureNode>(node);
                for (auto childNode: procedureNode->stmtList) {
                    curr_line_no = getNodePtrToLineNoMapHelper(childNode, map, curr_line_no);
                }
                break;
            }
            case IF_NODE: {
                shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(node);
                map->insert({ifNode, curr_line_no});
                curr_line_no++;
                for (auto childNode: ifNode->ifStmtList) {
                    curr_line_no = getNodePtrToLineNoMapHelper(childNode, map, curr_line_no);
                }
                for (auto childNode: ifNode->elseStmtList) {
                    curr_line_no = getNodePtrToLineNoMapHelper(childNode, map, curr_line_no);
                }
                break;
            }
            case WHILE_NODE: {
                shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(node);
                map->insert({whileNode, curr_line_no});
                curr_line_no++;
                for (auto childNode: whileNode->stmtList) {
                    curr_line_no = getNodePtrToLineNoMapHelper(childNode, map, curr_line_no);
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


} // AST