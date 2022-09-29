//
// Created by Nafour on 11/9/22.
//

#include "UsesExtractor.h"
#include "AST/utils/ASTUtils.h"
#include "utils/Utils.h"
#include "queue"
#include "EntityExtractor.h"
#include "CallsExtractor.h"

namespace DE {
    shared_ptr<list<vector<string>>> UsesExtractor::extractUsesS(shared_ptr<ProgramNode> rootPtr) {
        shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers = ASTUtils::getNodePtrToLineNumMap(rootPtr);
        shared_ptr<vector<vector<vector<string>>>> resultOfProcedures = make_shared<vector<vector<vector<string>>>>();
        shared_ptr<list<vector<string>>> ans = make_shared<list<vector<string>>>();

        for (auto procedureNode : rootPtr->procedureList) {
            shared_ptr<vector<vector<string>>> result = make_shared<vector<vector<string>>>();
            extractUsesSHelper(procedureNode, result, stmtNumbers);

            //save Uses relation from each procedure in result
            auto it = ans->begin();
            for (auto pair : *result) {
                it = ans->insert(it, pair);
            }
        }
        insertCallsForUseS(rootPtr, ans);
        insertCallsInIfAndWhileForUseS(rootPtr, ans);
        return ans;
    }

    unordered_set<string> UsesExtractor::extractUsesSHelper(shared_ptr<ASTNode> node,
        shared_ptr<vector<vector<string>>> result,
        shared_ptr<unordered_map<shared_ptr<StmtNode>, int>>
        stmtNumbers) {
        NodeType nodeType = ASTUtils::getNodeType(node);

        if (nodeType == AST::PROCEDURE_NODE) {
            shared_ptr<ProcedureNode> procedureNode1 = dynamic_pointer_cast<ProcedureNode>(node);
            unordered_set<string> usedVarsProcedure = unordered_set<string>();
            for (auto stmtNode : procedureNode1->stmtList) {
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
            result->insert(result->begin(), { to_string(stmtNum), var });
            return unordered_set<string>({ var });
        }
        case AST::IF_NODE: {
            shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(node);
            unordered_set<string> usedVarsIf = unordered_set<string>();

            for (auto child : ifNode->ifStmtList) {
                auto fromChild = extractUsesSHelper(child, result, stmtNumbers);
                usedVarsIf.insert(fromChild.begin(), fromChild.end());
            }

            for (auto child : ifNode->elseStmtList) {
                auto fromChild = extractUsesSHelper(child, result, stmtNumbers);
                usedVarsIf.insert(fromChild.begin(), fromChild.end());
            }

            auto variables = Utils::getVariablesFromExprString(ifNode->condExpr->condExpr);
            usedVarsIf.insert(variables.begin(), variables.end());

            for (const auto& usedVarIf : usedVarsIf) {
                int stmtNum = stmtNumbers->at(ifNode);
                result->insert(result->begin(), { to_string(stmtNum), usedVarIf });
            }
            return usedVarsIf;
        }
        case AST::WHILE_NODE: {
            shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(node);
            unordered_set<string> usedVarsWhile = unordered_set<string>();

            for (auto child : whileNode->stmtList) {
                auto fromChild = extractUsesSHelper(child, result, stmtNumbers);
                usedVarsWhile.insert(fromChild.begin(), fromChild.end());
            }

            auto variables = Utils::getVariablesFromExprString(whileNode->condExpr->condExpr);
            usedVarsWhile.insert(variables.begin(), variables.end());

            for (const auto& usedVarWhile : usedVarsWhile) {
                int stmtNum = stmtNumbers->at(whileNode);
                result->insert(result->begin(), { to_string(stmtNum), usedVarWhile });
            }
            return usedVarsWhile;

        }
        case AST::ASSIGN_NODE: {
            shared_ptr<AssignNode> assignNode = dynamic_pointer_cast<AssignNode>(node);
            auto variables = EntityExtractor::getVariablesFromExprNode(assignNode->expressionNode);
            for (const auto& var : variables) {
                int stmtNum = stmtNumbers->at(assignNode);
                result->insert(result->begin(), { to_string(stmtNum), var });
            }
            return variables;
        }
        default: {
            return unordered_set<string>();
        }
        }
    }

    void UsesExtractor::insertCallsForUseS(shared_ptr<ProgramNode> rootPtr, shared_ptr<list<vector<string>>> ans) {
        auto mappedProceduresToUsedVar = mapProceduresToUsedVariables(rootPtr);
        auto mappedCallNodesToProcedures = EntityExtractor::extractCallNodesFromProcedures(rootPtr);
        shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers = ASTUtils::getNodePtrToLineNumMap(rootPtr);

        for (auto pair : mappedCallNodesToProcedures) {
            vector<shared_ptr<CallNode>> listOfCallNodes = pair.second;
            for (auto callNode : listOfCallNodes) {
                int stmtNo = stmtNumbers->at(callNode);
                CallsExtractor::extractCallStmtRelationshipsToOutput(stmtNo, callNode,
                    mappedProceduresToUsedVar, mappedCallNodesToProcedures, ans);
            }
        }
    }

    shared_ptr<list<vector<string>>> UsesExtractor::extractUsesP(shared_ptr<ProgramNode> rootPtr) {
        shared_ptr<list<vector<string>>> ans = make_shared<list<vector<string>>>();
        auto mappedProceduresToUsedVar = mapProceduresToUsedVariables(rootPtr);
        auto mappedCallNodesToProcedures = EntityExtractor::extractCallNodesFromProcedures(rootPtr);
        for (auto pair : mappedProceduresToUsedVar) {
            unordered_set<string> uniqueVarList;
            string procedureName = pair.first;
            auto currentUsedVarList = pair.second;
            uniqueVarList.insert(currentUsedVarList.begin(), currentUsedVarList.end());

            // if the procedures has call nodes, handle them
            if (mappedCallNodesToProcedures.count(procedureName) != 0) {
                auto callNodes = mappedCallNodesToProcedures.at(procedureName);
                for (auto node : callNodes) {
                    EntityExtractor::extractVariablesFromCallNodesInProceduresToList(
                        node,
                        mappedProceduresToUsedVar,
                        mappedCallNodesToProcedures, uniqueVarList);
                }
            }

            for (auto v : uniqueVarList) {
                vector<string> usePEntry;
                usePEntry.push_back(procedureName);
                usePEntry.push_back(v);
                ans->push_back(usePEntry);
            }
        }

        return ans;
    }

    unordered_map<string, unordered_set<string>>
        UsesExtractor::mapProceduresToUsedVariables(shared_ptr<ProgramNode> rootPtr) {
        shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers = ASTUtils::getNodePtrToLineNumMap(rootPtr);
        shared_ptr<vector<vector<vector<string>>>> resultOfProcedures = make_shared<vector<vector<vector<string>>>>();
        auto map = unordered_map<string, unordered_set<string>>();

        for (auto procedureNode : rootPtr->procedureList) {
            shared_ptr<vector<vector<string>>> result = make_shared<vector<vector<string>>>();
            extractUsesSHelper(procedureNode, result, stmtNumbers);
            string procedureName = procedureNode->procedureName;
            unordered_set<string> variableList;
            for (auto pair : *result) {
                variableList.insert(pair[1]);
            }
            map.insert(make_pair(procedureName, variableList));
        }

        return map;
    }

    unordered_map<string, unordered_set<string>>
        UsesExtractor::mapIfAndWhileStmtNoToUsedVariables(shared_ptr<ProgramNode> rootPtr) {
        shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers = ASTUtils::getNodePtrToLineNumMap(rootPtr);
        auto map = unordered_map<string, unordered_set<string>>();

        auto ifAndWhileNodeList = EntityExtractor::extractIfAndWhileNodesFromProcedures(rootPtr);

        for (auto node : ifAndWhileNodeList) {
            auto uniqueVarList = unordered_set<string>();
            NodeType nodeType = ASTUtils::getNodeType(node);
            int stmtNo = stmtNumbers->at(node);
            queue<shared_ptr<StmtNode>> queue;
            queue.push(node);
            while (!queue.empty()) {
                switch (nodeType) {
                case AST::IF_NODE: {
                    shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(node);
                    auto variables = Utils::getVariablesFromExprString(ifNode->condExpr->condExpr);
                    uniqueVarList.insert(variables.begin(), variables.end());
                    for (auto n : ifNode->ifStmtList) {
                        queue.push(n);
                    }
                    for (auto n : ifNode->elseStmtList) {
                        queue.push(n);
                    }
                    break;
                }
                case AST::WHILE_NODE: {
                    shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(node);
                    auto variables = Utils::getVariablesFromExprString(whileNode->condExpr->condExpr);
                    uniqueVarList.insert(variables.begin(), variables.end());
                    for (auto n : whileNode->stmtList) {
                        queue.push(n);
                    }
                    break;
                }

                case AST::ASSIGN_NODE: {
                    shared_ptr<AssignNode> assignNode = dynamic_pointer_cast<AssignNode>(node);
                    auto variables = EntityExtractor::getVariablesFromExprNode(assignNode->expressionNode);
                    uniqueVarList.insert(variables.begin(), variables.end());
                    break;
                }

                case AST::PRINT_NODE: {
                    shared_ptr<PrintNode> printNode = dynamic_pointer_cast<PrintNode>(node);
                    string var = printNode->variableNode->variable;
                    uniqueVarList.insert(var);
                    break;
                }
                default:
                    break;
                }
            }

            map.insert(make_pair(to_string(stmtNo), uniqueVarList));
            return map;
        }

    } // DE

    void UsesExtractor::insertCallsInIfAndWhileForUseS(shared_ptr<ProgramNode> rootPtr, shared_ptr<list<vector<string>>> ans) {
        auto ifAndWhileNodeList = EntityExtractor::extractIfAndWhileNodesFromProcedures(rootPtr);
        shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers = ASTUtils::getNodePtrToLineNumMap(rootPtr);
        auto mappedProceduresToUsedVar = mapProceduresToUsedVariables(rootPtr);
        auto mappedCallNodesToProcedures = EntityExtractor::extractCallNodesFromProcedures(rootPtr);
        auto mappedIfAndWhileStmtNoToUsedVariables = mapIfAndWhileStmtNoToUsedVariables(rootPtr);

        if (!ifAndWhileNodeList.empty()) {
            for (auto node : ifAndWhileNodeList) {
                auto uniqueVarList = unordered_set<string>();
                NodeType nodeType = ASTUtils::getNodeType(node);
                int stmtNo = stmtNumbers->at(node);
                queue<shared_ptr<StmtNode>> queue;
                queue.push(node);
                while (!queue.empty()) {
                    switch (nodeType) {
                    case AST::CALL_NODE: {
                        shared_ptr<CallNode> callNode = dynamic_pointer_cast<CallNode>(node);
                        auto varList = mappedProceduresToUsedVar.at(callNode->procedureName);
                        auto usedVarList = mappedIfAndWhileStmtNoToUsedVariables.at(to_string(stmtNo));
                        for (auto var : varList) {
                            if (usedVarList.count(var) == 0) {
                                uniqueVarList.insert(var);
                            }
                        }

                        if (mappedCallNodesToProcedures.count(callNode->procedureName) != 0) {
                            auto otherCallNodes = mappedCallNodesToProcedures.at(callNode->procedureName);
                            for (auto n : otherCallNodes) {
                                queue.push(n);
                            }
                        }
                        break;
                    }

                    case AST::IF_NODE: {
                        shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(node);
                        for (auto n : ifNode->ifStmtList) {
                            queue.push(n);
                        }
                        for (auto n : ifNode->elseStmtList) {
                            queue.push(n);
                        }
                        break;
                    }
                    case AST::WHILE_NODE: {
                        shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(node);
                        for (auto n : whileNode->stmtList) {
                            queue.push(n);
                        }
                        break;
                    }

                    default:
                        break;
                    }
                }
                for (auto var : uniqueVarList) {
                    vector<string> callEntry;
                    callEntry.push_back(to_string(stmtNo));
                    callEntry.push_back(var);
                    ans->push_back(callEntry);
                }

            }
        }

       
    }
}
