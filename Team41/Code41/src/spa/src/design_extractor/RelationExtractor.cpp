//
// Created by hongshu wang on 10/9/22.
//

#include "AST/utils/ASTUtils.h"
#include "RelationExtractor.h"
#include <queue>
#include "utils/Utils.h"
#include "ParentExtractor.h"

namespace DE {
    void RelationExtractor::extractModifiesSDFS(shared_ptr<ASTNode> node,
                                                shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNoMap,
                                                shared_ptr<vector<string>> ancestors,
                                                shared_ptr<set<vector<string>>> output) {
        NodeType nodeType = ASTUtils::getNodeType(node);
        if (nodeType == NodeType::ASSIGN_NODE) {
            // add all {ancestor, variable} pairs to output
            shared_ptr<AssignNode> ptr = dynamic_pointer_cast<AssignNode>(node);
            int stmtNo = stmtNoMap->at(ptr);
            ancestors->push_back(to_string(stmtNo));
            for (string ancestor : *ancestors) {
                vector<string> row{ancestor, ptr->variableNode->variable};
                output->insert(row);
            }
            // remove current node from ancestors list
            ancestors->pop_back();
        } else if (nodeType == NodeType::READ_NODE) {
            // add all {ancestor, variable} pairs to output
            shared_ptr<ReadNode> ptr = dynamic_pointer_cast<ReadNode>(node);
            int stmtNo = stmtNoMap->at(ptr);
            ancestors->push_back(to_string(stmtNo));
            for (string ancestor : *ancestors) {
                vector<string> row{ancestor, ptr->variableNode->variable};
                output->insert(row);
            }
            // remove current node from ancestors list
            ancestors->pop_back();
        } else if (nodeType == NodeType::WHILE_NODE) {
            shared_ptr<WhileNode> ptr = dynamic_pointer_cast<WhileNode>(node);
            int stmtNo = stmtNoMap->at(ptr);
            ancestors->push_back(to_string(stmtNo));
            vector<shared_ptr<StmtNode>> stmtList = ptr->stmtList;
            for (shared_ptr<StmtNode> n: stmtList) {
                extractModifiesSDFS(n, stmtNoMap, ancestors, output);
            }
            // remove current node from ancestors list
            ancestors->pop_back();
        } else if (nodeType == NodeType::IF_NODE) {
            shared_ptr<IfNode> ptr = dynamic_pointer_cast<IfNode>(node);
            int stmtNo = stmtNoMap->at(ptr);
            ancestors->push_back(to_string(stmtNo));
            vector<shared_ptr<StmtNode>> ifStmtList = ptr->ifStmtList;
            vector<shared_ptr<StmtNode>> elseStmtList = ptr->elseStmtList;
            for (shared_ptr<StmtNode> n: ifStmtList) {
                extractModifiesSDFS(n, stmtNoMap, ancestors, output);
            }
            for (shared_ptr<StmtNode> n: elseStmtList) {
                extractModifiesSDFS(n, stmtNoMap, ancestors, output);
            }
            // remove current node from ancestors list
            ancestors->pop_back();
        }
    }

    shared_ptr<list<vector<string>>> RelationExtractor::extractModifiesS(shared_ptr<ProgramNode> rootPtr) {
        // each row is a vector<string>{stmt_no, variable_name}
        set<vector<string>> outputSet;
        shared_ptr<set<vector<string>>> outputSetPtr = make_shared<set<vector<string>>>(outputSet);
        shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers = ASTUtils::getNodePtrToLineNumMap(rootPtr);
        queue<shared_ptr<ASTNode>> queue;
        queue.push(rootPtr);
        vector<string> ancestors;
        shared_ptr<vector<string>> ancestorsPtr = make_shared<vector<string>>(ancestors);

        while (!queue.empty()) {
            shared_ptr<ASTNode> current = queue.front();
            queue.pop();

            NodeType nodeType = ASTUtils::getNodeType(current);
            if (nodeType == NodeType::ASSIGN_NODE) {
                // encounter an assign node, add modifiesS relation
                shared_ptr<AssignNode> ptr = dynamic_pointer_cast<AssignNode>(current);
                int stmtNo = stmtNumbers->at(ptr);
                shared_ptr<VariableNode> variableNode = ptr->variableNode;
                vector<string> row{to_string(stmtNo), variableNode->variable};
                outputSetPtr->insert(row);
            } else if (nodeType == NodeType::READ_NODE) {
                // encounter a read node, add modifiesS relation
                shared_ptr<ReadNode> ptr = dynamic_pointer_cast<ReadNode>(current);
                int stmtNo = stmtNumbers->at(ptr);
                shared_ptr<VariableNode> variableNode = ptr->variableNode;
                vector<string> row{to_string(stmtNo), variableNode->variable};
                outputSetPtr->insert(row);
            } else if (nodeType == NodeType::WHILE_NODE) {
                // encounter a while node, check stmtList
                shared_ptr<WhileNode> ptr = dynamic_pointer_cast<WhileNode>(current);
                extractModifiesSDFS(ptr, stmtNumbers, ancestorsPtr, outputSetPtr);
            } else if (nodeType == NodeType::IF_NODE) {
                // encounter an if node, check ifStmts and elseStmts
                shared_ptr<IfNode> ptr = dynamic_pointer_cast<IfNode>(current);
                extractModifiesSDFS(ptr, stmtNumbers, ancestorsPtr, outputSetPtr);
            } else if (nodeType == NodeType::PROCEDURE_NODE) {
                // encounter a procedure node, check its statements
                shared_ptr<ProcedureNode> ptr = dynamic_pointer_cast<ProcedureNode>(current);
                vector<shared_ptr<StmtNode>> stmtList = ptr->stmtList;
                for (shared_ptr<StmtNode> n: stmtList) {
                    queue.push(n);
                }
            } else if (nodeType == NodeType::PROGRAM_NODE) {
                // encounter a program node, check its procedures
                shared_ptr<ProgramNode> ptr = dynamic_pointer_cast<ProgramNode>(current);
                vector<shared_ptr<ProcedureNode>> procedureList = ptr->procedureList;
                for (shared_ptr<ProcedureNode> n: procedureList) {
                    queue.push(n);
                }
            }
        }

        // convert set to list
        list<vector<string>> outputList;
        shared_ptr<list<vector<string>>> output = make_shared<list<vector<string>>>(outputList);
        auto outputIterator = outputSetPtr->begin();
        while (outputIterator != outputSetPtr->end()) {
            output->push_back(*outputIterator);
            advance(outputIterator, 1);
        }

        return output;
    }

    list<vector<string>> RelationExtractor::extractRelation(
            shared_ptr<ProgramNode> rootPtr, RelationType relationType) {
        switch (relationType) {
            case RelationType::PARENT: {
                return *ParentExtractor::extractParent(rootPtr);
            }
            case RelationType::PARENT_T: {
                return *ParentExtractor::extractParentT(rootPtr);
            }
            case RelationType::MODIFIES_S: {
                return *extractModifiesS(rootPtr);
            }
            case RelationType::USES_S: {
                return *extractUsesS(rootPtr);
            }
            default: {
                list<vector<string>> emptyList;
                return emptyList;
            }
        }
    }

    shared_ptr<list<vector<string>>> RelationExtractor::extractUsesS(shared_ptr<ProgramNode> rootPtr) {
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

    unordered_set<string> RelationExtractor::extractUsesSHelper(shared_ptr<ASTNode> node, shared_ptr<vector<vector<string>>> result, shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers) {
        NodeType nodeType = ASTUtils::getNodeType(node);

        if (nodeType == AST::PROCEDURE_NODE) {
            shared_ptr<ProcedureNode> procedureNode1 = dynamic_pointer_cast<ProcedureNode>(node);

            for (auto stmtNode: procedureNode1->stmtList) {
                extractUsesSHelper(stmtNode, result, stmtNumbers);
            }

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
                auto variables = Utils::getVariablesFromExprString(assignNode->expressionNode->expr);
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
}
