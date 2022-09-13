//
// Created by Nafour on 11/9/22.
//

#include "UsesExtractor.h"
#include "AST/utils/ASTUtils.h"
#include "utils/Utils.h"
#include "EntityExtractor.h"

namespace DE {
    shared_ptr<list<vector<string>>> UsesExtractor::extractUsesS(shared_ptr<ProgramNode> rootPtr) {
        shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers = ASTUtils::getNodePtrToLineNumMap(rootPtr);
        shared_ptr<vector<vector<vector<string>>>> resultOfProcedures = make_shared<vector<vector<vector<string>>>>();
        shared_ptr<list<vector<string>>> ans = make_shared<list<vector<string>>>();

        for (auto procedureNode: rootPtr->procedureList) {
            shared_ptr<vector<vector<string>>> result = make_shared<vector<vector<string>>>();
            extractUsesSHelper(procedureNode, result, stmtNumbers);

            //save Uses relation from each procedure in result
            auto it = ans->begin();
            for (auto pair: *result) {
                it = ans->insert(it, pair);
            }
        }
        return ans;
    }

    unordered_set<string> UsesExtractor::extractUsesSHelper(shared_ptr<ASTNode> node, shared_ptr<vector<vector<string>>> result, shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers) {
        NodeType nodeType = ASTUtils::getNodeType(node);

        if (nodeType == AST::PROCEDURE_NODE) {
            shared_ptr<ProcedureNode> procedureNode1 = dynamic_pointer_cast<ProcedureNode>(node);
            unordered_set<string> usedVarsProcedure = unordered_set<string>();
            for (auto stmtNode: procedureNode1->stmtList) {
                auto fromChild = extractUsesSHelper(stmtNode, result, stmtNumbers);
                usedVarsProcedure.insert(fromChild.begin(), fromChild.end());
            }
            return usedVarsProcedure;
        }

        switch (nodeType) {
            case AST::PRINT_NODE: {
                shared_ptr<PrintNode> printNode = dynamic_pointer_cast<PrintNode>(node);
                int stmtNum = stmtNumbers->at(printNode);
                string var = printNode->variableNode->variable;
                result->insert(result->begin(), {to_string(stmtNum), var});
                return unordered_set<string>({var});
            }
            case AST::IF_NODE: {
                shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(node);
                unordered_set<string> usedVarsIf = unordered_set<string>();

                for (auto child: ifNode->ifStmtList) {
                    auto fromChild = extractUsesSHelper(child, result, stmtNumbers);
                    usedVarsIf.insert(fromChild.begin(), fromChild.end());
                }

                for (auto child: ifNode->elseStmtList) {
                    auto fromChild = extractUsesSHelper(child, result, stmtNumbers);
                    usedVarsIf.insert(fromChild.begin(), fromChild.end());
                }

                auto variables = Utils::getVariablesFromExprString(ifNode->condExpr->condExpr);
                usedVarsIf.insert(variables.begin(), variables.end());

                for (const auto& usedVarIf: usedVarsIf) {
                    int stmtNum = stmtNumbers->at(ifNode);
                    result->insert( result->begin() , {to_string(stmtNum), usedVarIf} );
                }
                return usedVarsIf;
            }
            case AST::WHILE_NODE: {
                shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(node);
                unordered_set<string> usedVarsWhile = unordered_set<string>();

                for (auto child: whileNode->stmtList) {
                    auto fromChild = extractUsesSHelper(child, result, stmtNumbers);
                    usedVarsWhile.insert(fromChild.begin(), fromChild.end());
                }

                auto variables = Utils::getVariablesFromExprString(whileNode->condExpr->condExpr);
                usedVarsWhile.insert(variables.begin(), variables.end());

                for (const auto& usedVarWhile: usedVarsWhile) {
                    int stmtNum = stmtNumbers->at(whileNode);
                    result->insert( result->begin() , {to_string(stmtNum), usedVarWhile} );
                }
                return usedVarsWhile;

            }
            case AST::ASSIGN_NODE: {
                shared_ptr<AssignNode> assignNode = dynamic_pointer_cast<AssignNode>(node);
                auto variables = EntityExtractor::getVariablesFromExprNode(assignNode->expressionNode);
                for (const auto& var: variables) {
                    int stmtNum = stmtNumbers->at(assignNode);
                    result->insert(result->begin(), {to_string(stmtNum), var});
                }
                return variables;
            }
            default: {
                return unordered_set<string>();
            }
        }
    }
} // DE