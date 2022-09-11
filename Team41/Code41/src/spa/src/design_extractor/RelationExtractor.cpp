//
// Created by hongshu wang on 10/9/22.
//

#include "AST/utils/ASTUtils.h"
#include "RelationExtractor.h"
#include <queue>
#include "utils/Utils.h"

namespace DE {
    shared_ptr<map<int, vector<int>>> RelationExtractor::extractParentHashmap(shared_ptr<ProgramNode> rootPtr) {
        map<int, vector<int>> parentRelations;
        shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers = ASTUtils::getNodePtrToLineNumMap(rootPtr);
        queue<shared_ptr<ASTNode>> queue;
        queue.push(rootPtr);

        while (!queue.empty()) {
            shared_ptr<ASTNode> current = queue.front();
            queue.pop();

            NodeType nodeType = ASTUtils::getNodeType(current);
            if (nodeType == NodeType::WHILE_NODE) {
                shared_ptr<WhileNode> ptr = dynamic_pointer_cast<WhileNode>(current);
                vector<shared_ptr<StmtNode>> stmtList = ptr->stmtList;
                int parent = stmtNumbers->at(ptr);
                vector<int> children;
                for (shared_ptr<StmtNode> n: stmtList) {
                    queue.push(n);
                    int child = stmtNumbers->at(n);
                    children.push_back(child);
                }
                pair<int, vector<int>> row{parent, children};
                parentRelations.insert(row);
            } else if (nodeType == NodeType::IF_NODE) {
                shared_ptr<IfNode> ptr = dynamic_pointer_cast<IfNode>(current);
                vector<shared_ptr<StmtNode>> ifStmtList = ptr->ifStmtList;
                vector<shared_ptr<StmtNode>> elseStmtList = ptr->elseStmtList;
                int parent = stmtNumbers->at(ptr);
                vector<int> children;
                for (shared_ptr<StmtNode> n: ifStmtList) {
                    queue.push(n);
                    int child = stmtNumbers->at(n);
                    children.push_back(child);
                }
                for (shared_ptr<StmtNode> n: elseStmtList) {
                    queue.push(n);
                    int child = stmtNumbers->at(n);
                    children.push_back(child);
                }
                pair<int, vector<int>> row{parent, children};
                parentRelations.insert(row);
            } else if (nodeType == NodeType::PROGRAM_NODE) {
                // encounter a program node, check its procedures
                shared_ptr<ProgramNode> ptr = dynamic_pointer_cast<ProgramNode>(current);
                vector<shared_ptr<ProcedureNode>> procedureList = ptr->procedureList;
                for (shared_ptr<ProcedureNode> n: procedureList) {
                    queue.push(n);
                }
            } else if (nodeType == NodeType::PROCEDURE_NODE) {
                // encounter a procedure node, check its statements
                shared_ptr<ProcedureNode> ptr = dynamic_pointer_cast<ProcedureNode>(current);
                vector<shared_ptr<StmtNode>> stmtList = ptr->stmtList;
                for (shared_ptr<StmtNode> n: stmtList) {
                    queue.push(n);
                }
            }
        }

        return make_shared<map<int, vector<int>>>(parentRelations);
    }

    shared_ptr<list<vector<string>>> RelationExtractor::extractParent(shared_ptr<ProgramNode> rootPtr) {
        list<vector<string>> output;
        shared_ptr<map<int, vector<int>>> parentRelations = extractParentHashmap(rootPtr);
        auto hashmapIterator = parentRelations->begin();
        while (hashmapIterator != parentRelations->end()) {
            string parent = to_string(hashmapIterator->first);
            vector<int> children = hashmapIterator->second;
            for (int child : children) {
                vector<string> row{parent, to_string(child)};
                output.push_back(row);
            }
            advance(hashmapIterator, 1);
        }

        return make_shared<list<vector<string>>>(output);
    }

    void RelationExtractor::extractParentTDFS(shared_ptr<map<int, vector<int>>> parentRelations, int key,
                                              shared_ptr<vector<string>> ancestors,
                                              shared_ptr<list<vector<string>>> output) {
        if (parentRelations->find(key) == parentRelations->end()) {
            return;
        }
        // add current stmt to ancestors
        ancestors->push_back(to_string(key));
        // pop current stmt's children
        vector<int> children = parentRelations->find(key)->second;
        parentRelations->erase(key);
        for (string ancestor : *ancestors) {
            for (int child : children) {
                // add combinations to outputs
                vector<string> row{ancestor, to_string(child)};
                output->push_back(row);
                // dfs
                extractParentTDFS(parentRelations, child, ancestors, output);
            }
        }
        // remove current stmt from ancestors
        ancestors->pop_back();
    }

    shared_ptr<list<vector<string>>> RelationExtractor::extractParentT(shared_ptr<AST::ProgramNode> rootPtr) {
        list<vector<string>> outputList;
        shared_ptr<list<vector<string>>> output = make_shared<list<vector<string>>>(outputList);
        shared_ptr<map<int, vector<int>>> parentRelations = extractParentHashmap(rootPtr);
        vector<string> ancestors;
        shared_ptr<vector<string>> ancestorsPtr = make_shared<vector<string>>(ancestors);

        while (!parentRelations->empty()) {
            auto iterator = parentRelations->begin();
            int key = iterator->first;
            extractParentTDFS(parentRelations, key, ancestorsPtr, output);
        }

        return output;
    }

    list<vector<string>> RelationExtractor::extractRelation(
            shared_ptr<ProgramNode> rootPtr, RelationType relationType) {
        switch (relationType) {
            case RelationType::PARENT: {
                return *extractParent(rootPtr);
            case RelationType::PARENT_T:
                return *extractParentT(rootPtr);
            }
            case RelationType::USES_S: {
                return *extractUsesS(rootPtr);
            }
            default:
                list<vector<string>> emptyList;
                return emptyList;
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
