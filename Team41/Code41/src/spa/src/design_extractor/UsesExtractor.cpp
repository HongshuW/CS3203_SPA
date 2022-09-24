//
// Created by Nafour on 11/9/22.
//

#include "UsesExtractor.h"
#include "AST/utils/ASTUtils.h"
#include "utils/Utils.h"
#include "queue"
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
        insertUseCalls(rootPtr, ans);
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

    shared_ptr<list<vector<string>>> UsesExtractor::extractUsesP(shared_ptr<ProgramNode> rootPtr) {
        shared_ptr<list<vector<string>>> ans = make_shared<list<vector<string>>>();
        auto map = mapProceduresToUsedVariables(rootPtr);
        for (auto pair: map){
            string procedureName = pair.first;
            unordered_set<string> variableList = pair.second;
            for (auto var: variableList) {
                vector<string> usePEntry;
                usePEntry.push_back(procedureName);
                usePEntry.push_back(var);
                ans->push_back(usePEntry);
            }
        }
        return ans;
    }

    unordered_map<string, unordered_set<string>>
            UsesExtractor::mapProceduresToUsedVariables(shared_ptr<ProgramNode> rootPtr) {
        shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers = ASTUtils::getNodePtrToLineNumMap(rootPtr);
        shared_ptr<vector<vector<vector<string>>>> resultOfProcedures = make_shared<vector<vector<vector<string>>>>();
        shared_ptr<list<vector<string>>> ans = make_shared<list<vector<string>>>();
        auto map = unordered_map<string, unordered_set<string>>();

        for (auto procedureNode: rootPtr->procedureList) {
            shared_ptr<vector<vector<string>>> result = make_shared<vector<vector<string>>>();
            extractUsesSHelper(procedureNode, result, stmtNumbers);
            string procedureName = procedureNode -> procedureName;
            unordered_set<string> variableList;
            for (auto pair: *result) {
                variableList.insert(pair[1]);
            }
            map.insert(make_pair(procedureName, variableList));
        }

        return map;
    }

    unordered_map<string, vector<shared_ptr<CallNode>>>
            UsesExtractor::getCallNodesFromProcedure(shared_ptr<ProgramNode> rootPtr) {
        auto listOfCallNodesFromProcedures = list<vector<shared_ptr<CallNode>>>();
        auto mapCallNodesToProcedures = unordered_map<string, vector<shared_ptr<CallNode>>>();
        vector<shared_ptr<ProcedureNode>> procedureList = rootPtr -> procedureList;

        for (auto procedureNode: procedureList) {
            string name = procedureNode->procedureName;
            auto listOfCallNodes = vector<shared_ptr<CallNode>>();
            queue<vector<shared_ptr<StmtNode>>> queue;
            queue.push(procedureNode->stmtList);
            while(!queue.empty()) {
                auto stmtList = queue.front();
                queue.pop();
                for (auto stmtNode: stmtList) {
                    NodeType nodeType = ASTUtils::getNodeType(stmtNode);
                    switch (nodeType) {
                        case AST::CALL_NODE: {
                            shared_ptr<CallNode> callNode = dynamic_pointer_cast<CallNode>(stmtNode);
                            listOfCallNodes.push_back(callNode);
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
            if(!listOfCallNodes.empty()) {
                mapCallNodesToProcedures.insert(make_pair(name, listOfCallNodes));
            }
        }
        return mapCallNodesToProcedures;
    }

    void UsesExtractor::insertUseCalls(shared_ptr<ProgramNode> rootPtr, shared_ptr<list<vector<string>>> ans) {
        auto mappedProceduresToUsedVar = mapProceduresToUsedVariables(rootPtr);
        auto mappedCallNodesToProcedures = getCallNodesFromProcedure(rootPtr);
        shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers = ASTUtils::getNodePtrToLineNumMap(rootPtr);

        auto mappedReachableCallNodesToProcedures = unordered_map<string, vector<shared_ptr<CallNode>>>();
        for (auto pair: mappedCallNodesToProcedures){
            string procedureName = pair.first;
            vector<shared_ptr<CallNode>> listOfCallNodes = pair.second;
            for(auto callNode: listOfCallNodes) {
                int stmtNo = stmtNumbers->at(callNode);
                queue<shared_ptr<CallNode>> queue;
                queue.push(callNode);
                while(!queue.empty()) {
                    auto callNodeEntry = queue.front();
                    queue.pop();
                    auto varList = mappedProceduresToUsedVar.at(callNodeEntry->procedureName);
                    for (auto var: varList) {
                        vector<string> useCallEntry;
                        useCallEntry.push_back(to_string(stmtNo));
                        useCallEntry.push_back(var);
                        ans -> push_back(useCallEntry);
                    }

                    if (mappedCallNodesToProcedures.count(callNodeEntry->procedureName) != 0) {
                        auto otherCallNodes = mappedCallNodesToProcedures.at(callNodeEntry-> procedureName);
                        for (auto n: otherCallNodes) {
                            queue.push(n);
                        }
                    }
                }
            }
        }
    }


} // DE