//
// Created by hongshu wang on 10/9/22.
//

#include "AST/utils/ASTUtils.h"
#include "RelationExtractor.h"
#include "FollowsExtractor.h"
#include <queue>

namespace DE {
    unordered_map<int, vector<int>> RelationExtractor::extractParentHashmap(shared_ptr<ProgramNode> rootPtr) {
        unordered_map<int, vector<int>> parentRelations;
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

        return parentRelations;
    }
    shared_ptr<list<vector<string>>> RelationExtractor::extractParent(shared_ptr<ProgramNode> rootPtr) {
        list<vector<string>> output;
        unordered_map<int, vector<int>> parentRelations = extractParentHashmap(rootPtr);
        auto hashmapIterator = parentRelations.begin();
        while (hashmapIterator != parentRelations.end()) {
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

    list<vector<string>> RelationExtractor::extractRelation(
            shared_ptr<ProgramNode> rootPtr, RelationType relationType) {
        switch (relationType) {
            case RelationType::PARENT:
                return *extractParent(rootPtr);
            case RelationType::FOLLOWS:
                return *FollowsExtractor().extractFollows(rootPtr);
            case RelationType::FOLLOWS_T:
                return *FollowsExtractor().extractFollowsStar(rootPtr);
            default:
                list<vector<string>> emptyList;
                return emptyList;
        }
    }
}
