//
// Created by hongshu wang on 10/9/22.
//

#include "AST/utils/ASTUtils.h"
#include "RelationExtractor.h"
#include <queue>

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
            case RelationType::PARENT:
                return *extractParent(rootPtr);
            case RelationType::PARENT_T:
                return *extractParentT(rootPtr);
            case RelationType::MODIFIES_S:
                return *extractModifiesS(rootPtr);
            default:
                list<vector<string>> emptyList;
                return emptyList;
        }
    }
}
