//
// Created by hongshu wang on 11/9/22.
//

#include "AST/utils/ASTUtils.h"
#include "ParentExtractor.h"
#include <queue>

shared_ptr<map<int, vector<int>>> ParentExtractor::extractParentHashmap(shared_ptr<ProgramNode> rootPtr) {
    map<int, vector<int>> parentRelations;
    shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers = ASTUtils::getNodePtrToLineNumMap(rootPtr);
    queue<shared_ptr<ASTNode>> queue;
    queue.push(rootPtr);

    while (!queue.empty()) {
        shared_ptr<ASTNode> current = queue.front();
        queue.pop();

        NodeType nodeType = ASTUtils::getNodeType(current);
        switch(nodeType) {
            case AST::WHILE_NODE: {
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
                break;
            }
            case AST::IF_NODE: {
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
                break;
            }
            case AST::PROGRAM_NODE: {
                // encounter a program node, check its procedures
                shared_ptr<ProgramNode> ptr = dynamic_pointer_cast<ProgramNode>(current);
                vector<shared_ptr<ProcedureNode>> procedureList = ptr->procedureList;
                for (shared_ptr<ProcedureNode> n: procedureList) {
                    queue.push(n);
                }
                break;
            }
            case AST::PROCEDURE_NODE: {
                // encounter a procedure node, check its statements
                shared_ptr<ProcedureNode> ptr = dynamic_pointer_cast<ProcedureNode>(current);
                vector<shared_ptr<StmtNode>> stmtList = ptr->stmtList;
                for (shared_ptr<StmtNode> n: stmtList) {
                    queue.push(n);
                }
                break;
            }
            default:
                break;
        }
    }

    return make_shared<map<int, vector<int>>>(parentRelations);
}

void ParentExtractor::extractParentTDFS(shared_ptr<map<int, vector<int>>> parentRelations, int key,
                                          vector<string> ancestors,
                                          shared_ptr<list<vector<string>>> output) {
    if (parentRelations->count(key) == 0) {
        return;
    }
    // add current stmt to ancestors
    ancestors.push_back(to_string(key));
    // pop current stmt's children
    vector<int> children = parentRelations->at(key);
    parentRelations->erase(key);
    for (string ancestor : ancestors) {
        for (int child : children) {
            // add combinations to outputs
            vector<string> row{ancestor, to_string(child)};
            output->push_back(row);
            // dfs
            extractParentTDFS(parentRelations, child, ancestors, output);
        }
    }
}

shared_ptr<list<vector<string>>> ParentExtractor::extractParent(shared_ptr<ProgramNode> rootPtr) {
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

shared_ptr<list<vector<string>>> ParentExtractor::extractParentT(shared_ptr<AST::ProgramNode> rootPtr) {
    list<vector<string>> outputList;
    shared_ptr<list<vector<string>>> output = make_shared<list<vector<string>>>(outputList);
    shared_ptr<map<int, vector<int>>> parentRelations = extractParentHashmap(rootPtr);
    vector<string> ancestors;

    while (!parentRelations->empty()) {
        auto iterator = parentRelations->begin();
        int key = iterator->first;
        extractParentTDFS(parentRelations, key, ancestors, output);
    }

    return output;
}
