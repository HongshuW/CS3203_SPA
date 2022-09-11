//
// Created by hongshu wang on 11/9/22.
//

#include "AST/utils/ASTUtils.h"
#include "ModifiesExtractor.h"
#include <queue>

void ModifiesExtractor::extractModifiesSDFS(shared_ptr<ASTNode> node,
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

shared_ptr<list<vector<string>>> ModifiesExtractor::extractModifiesS(shared_ptr<ProgramNode> rootPtr) {
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

