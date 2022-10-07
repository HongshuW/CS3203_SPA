//
// Created by Nafour on 11/9/22.
//

#include "PatternExtractor.h"
#include "AST/utils/ASTUtils.h"
#include <queue>
namespace DE {

    vector<pair<pair<int, string>, std::shared_ptr<AssignNode>>>
    PatternExtractor::extractPattern(shared_ptr<ProgramNode> programNode) {
        shared_ptr<vector<pair<pair<int, string>, std::shared_ptr<AssignNode>>>> resultVec = make_shared<vector<pair<pair<int, string>, std::shared_ptr<AssignNode>>>>();
        auto stmtNumbers = ASTUtils::getNodePtrToLineNumMap(programNode);
        for (auto procedureNode: programNode->procedureList) {
            extractPatternHelper(procedureNode, resultVec, stmtNumbers);
        }
        return *resultVec;
    }

    void PatternExtractor::extractPatternHelper(shared_ptr<ASTNode> node,
                                                shared_ptr<vector<pair<pair<int, string>, std::shared_ptr<AssignNode>>>> resultVec,
                                                shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers) {
        NodeType nodeType = ASTUtils::getNodeType(node);

        if (nodeType == AST::PROCEDURE_NODE) {
            shared_ptr<ProcedureNode> procedureNode1 = dynamic_pointer_cast<ProcedureNode>(node);
            for (auto stmtNode: procedureNode1->stmtList) {
                extractPatternHelper(stmtNode, resultVec, stmtNumbers);
            }
            return;
        }
        switch (nodeType) {
            case AST::ASSIGN_NODE: {
                shared_ptr<AssignNode> assignNode = dynamic_pointer_cast<AssignNode>(node);
                int lineNum = stmtNumbers->at(assignNode);
                resultVec->insert(resultVec->begin(), pair(pair(lineNum, assignNode->variableNode->variable), assignNode));
                return;
            }
            case AST::IF_NODE: {
                shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(node);
                for (auto stmtNode: ifNode->ifStmtList) {
                    extractPatternHelper(stmtNode, resultVec, stmtNumbers);
                }
                for(auto stmtNode: ifNode->elseStmtList) {
                    extractPatternHelper(stmtNode, resultVec, stmtNumbers);
                }
                return;
            }
            case AST::WHILE_NODE: {
                shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(node);
                for(auto stmtNode: whileNode->stmtList) {
                    extractPatternHelper(stmtNode, resultVec, stmtNumbers);
                }
                return;
            }
        }

    }

    list<vector<string>> PatternExtractor::extractIfPattern(shared_ptr<ProgramNode> programNode) {
        list<vector<string>> output;
        auto stmtNumbers = ASTUtils::getNodePtrToLineNumMap(programNode);
        for (auto procedureNode: programNode -> procedureList) {
            queue<vector<shared_ptr<StmtNode>>> queue;
            queue.push(procedureNode->stmtList);
            while(!queue.empty()) {
                auto stmtList = queue.front();
                queue.pop();
                for (auto stmtNode: stmtList) {
                    NodeType nodeType = ASTUtils::getNodeType(stmtNode);
                    switch (nodeType) {
                        case AST::IF_NODE: {
                            shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(stmtNode);
                            int stmtNo = stmtNumbers->at(ifNode);
                            vector<string> varList = condExprNodeHandler(ifNode->condExpr);
                            for (string var: varList) {
                                vector<string> ifEntry;
                                ifEntry.push_back(to_string(stmtNo));
                                ifEntry.push_back(var);
                                output.push_back(ifEntry);
                            }

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
        }

        return output;
    }

    list<vector<string>> PatternExtractor::extractWhilePattern(shared_ptr<ProgramNode> programNode) {
        list<vector<string>> output;
        auto stmtNumbers = ASTUtils::getNodePtrToLineNumMap(programNode);
        for (auto procedureNode: programNode -> procedureList) {
            queue<vector<shared_ptr<StmtNode>>> queue;
            queue.push(procedureNode->stmtList);
            while(!queue.empty()) {
                auto stmtList = queue.front();
                queue.pop();
                for (auto stmtNode: stmtList) {
                    NodeType nodeType = ASTUtils::getNodeType(stmtNode);
                    switch (nodeType) {
                        case AST::WHILE_NODE: {
                            shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(stmtNode);
                            int stmtNo = stmtNumbers->at(whileNode);
                            vector<string> varList = condExprNodeHandler(whileNode->condExpr);
                            for (string var: varList) {
                                vector<string> whileEntry;
                                whileEntry.push_back(to_string(stmtNo));
                                whileEntry.push_back(var);
                                output.push_back(whileEntry);
                            }
                            vector<shared_ptr<StmtNode>> whileStmtList = whileNode->stmtList;
                            queue.push(whileStmtList);
                            break;
                        }

                        case AST::IF_NODE: {
                            shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(stmtNode);
                            vector<shared_ptr<StmtNode>> ifStmtList = ifNode->ifStmtList;
                            vector<shared_ptr<StmtNode>> elseStmtList = ifNode->elseStmtList;
                            queue.push(ifStmtList);
                            queue.push(elseStmtList);
                            break;
                        }
                        default:
                            break;
                    }
                }
            }
        }

        return output;
    }

    vector<string> PatternExtractor::condExprNodeHandler(shared_ptr<CondExprNode> condExpr) {
        vector<shared_ptr<RelExprNode>> relExprNodeList;
        vector<string> varList;

        if (condExpr->relExprNode != nullptr) {
            relExprNodeList.push_back(condExpr->relExprNode);
        }

        if (condExpr->condExprLHS != nullptr) {
            PatternExtractor::getRelExprNodesDFS(condExpr->condExprLHS, relExprNodeList);
        }

        if (condExpr->condExprRHS != nullptr) {
            PatternExtractor::getRelExprNodesDFS(condExpr->condExprRHS, relExprNodeList);
        }

        for (auto relExpr: relExprNodeList) {
            relExprNodeHandler(relExpr, varList);
        }
        return varList;
    }

    void PatternExtractor::relExprNodeHandler(shared_ptr<RelExprNode> relExpr,
                                                        vector<string>& varList) {
        auto exprLHS = relExpr->exprNodeLHS;
        auto exprRHS = relExpr->exprNodeRHS;
        exprNodeHandler(exprLHS, varList);
        exprNodeHandler(exprRHS, varList);
    }

    void PatternExtractor::exprNodeHandler(shared_ptr<ExprNode> expr, vector<string>& varList) {
        getVarFromExprNodesDFS(expr, varList);
    }

    void PatternExtractor::getVarFromExprNodesDFS(shared_ptr<ExprNode> expr, vector<string>& varList) {
        if (expr->isVariableNode()) {
            varList.push_back(expr->expr);
        }

        if (expr->isOperatorNode()) {
            auto LHS = expr->left;
            auto RHS = expr->right;
            getVarFromExprNodesDFS(LHS, varList);
            getVarFromExprNodesDFS(RHS, varList);
        }
    }

    void PatternExtractor::getRelExprNodesDFS(shared_ptr<CondExprNode> condExpr,
                                              vector<shared_ptr<RelExprNode>> &relExprNodeList) {
        if(condExpr->relExprNode != nullptr) {
            relExprNodeList.push_back(condExpr->relExprNode);
        }

        if (condExpr->condExprLHS != nullptr) {
            getRelExprNodesDFS(condExpr->condExprLHS, relExprNodeList);
        }

        if (condExpr->condExprRHS != nullptr) {
            getRelExprNodesDFS(condExpr->condExprRHS, relExprNodeList);
        }
    }
} // DE