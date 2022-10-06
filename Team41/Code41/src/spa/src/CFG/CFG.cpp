//
// Created by hongshu wang on 29/9/22.
//

#include "./AST/utils/ASTUtils.h"
#include "CFG.h"
#include <queue>

CFG::CFG(ProcedureNode procedureNode, shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers) {
    this->stmtNumbers = stmtNumbers;
    this->cfg = make_shared<cfgType>(cfgType());

    vector<shared_ptr<StmtNode>> stmtList = procedureNode.stmtList;
    processStmtList(stmtList);
}

vector<int> CFG::getStmtNoList(vector<shared_ptr<StmtNode>> stmtList) {
    int size = stmtList.size();
    vector<int> output = vector<int>(vector<int>());
    for (int i = 0; i < size; i++) {
        shared_ptr<StmtNode> stmt = stmtList[i];
        int stmtNumber = stmtNumbers->at(stmt);
        output.push_back(stmtNumber);
    }
    return output;
}

void CFG::processStmtList(vector<shared_ptr<StmtNode>> stmtList) {
    if (stmtList.empty()) {
        return;
    }
    // record the statement number for each statement in the list
    vector<int> stmtNoVector = getStmtNoList(stmtList);
    // add each statement to the cfg
    int size = stmtList.size();
    int previous = invalidStmtNo;
    int next;
    NodeType previousNodeType;
    for (int i = 0; i < size; i++) {
        shared_ptr<StmtNode> stmt = stmtList[i];
        int stmtNumber = stmtNoVector[i];
        if (i < size - 1) {
            next = stmtNoVector[i + 1];
        } else {
            next = invalidStmtNo;
        }
        if (i > 0 && previousNodeType == AST::IF_NODE) {
            // do not add next for if statement
            previous = invalidStmtNo;
        }
        processStmt(stmt, previous, next);
        previous = stmtNumber;
        previousNodeType = ASTUtils::getNodeType(stmt);
    }
    addEdgeToCFG(stmtNoVector[size - 1], invalidStmtNo);
}

void CFG::processStmt(shared_ptr<StmtNode> stmt, int previous, int next) {
    int stmtNumber = stmtNumbers->at(stmt);

    if (previous != invalidStmtNo) {
        addEdgeToCFG(previous, stmtNumber);
    }

    NodeType nodeType = ASTUtils::getNodeType(stmt);
    if (nodeType == AST::WHILE_NODE) {
        // add the statements inside the while loop to the cfg
        shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(stmt);
        processWhileStmtList(stmtNumber, whileNode->stmtList);
    } else if (nodeType == AST::IF_NODE) {
        // add the statements inside the if statement to the cfg
        shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(stmt);
        processIfStmtList(stmtNumber, next, ifNode->ifStmtList);
        processIfStmtList(stmtNumber, next, ifNode->elseStmtList);
    }
}

void CFG::addEdgeToCFG(int current, int next) {
    auto currentPtr = cfg->find(current);
    if (currentPtr == cfg->end()) {
        // if cfg doesn't contain key, add new key
        cfg->insert({current, unordered_set<int>()});
    }
    currentPtr = cfg->find(current);
    if (next != invalidStmtNo) {
        currentPtr->second.insert(next);
    }
}

void CFG::processWhileStmtList(int parent, vector<shared_ptr<StmtNode>> stmtList) {
    if (stmtList.empty()) {
        // loop with no statements, the statement points to itself.
        addEdgeToCFG(parent, parent);
        return;
    }
    // record the statement number for each statement in the list
    vector<int> stmtNoVector = getStmtNoList(stmtList);
    // add each statement to the cfg
    int size = stmtList.size();
    int previous = parent;
    int next;
    NodeType previousNodeType;
    for (int i = 0; i < size; i++) {
        shared_ptr<StmtNode> stmt = stmtList[i];
        int stmtNumber = stmtNoVector[i];
        if (i < size - 1) {
            next = stmtNoVector[i + 1];
        } else {
            // if this is the last statement in the while loop, it should link to its parent
            next = parent;
        }
        if (i > 0 && previousNodeType == AST::IF_NODE) {
            // do not add next for if statement
            previous = invalidStmtNo;
        }
        processStmt(stmt, previous, next);
        previous = stmtNumber;
        previousNodeType = ASTUtils::getNodeType(stmt);
    }
    if (previousNodeType != AST::IF_NODE) {
        addEdgeToCFG(stmtNoVector[size - 1], next);
    }
}

void CFG::processIfStmtList(int parent, int parentNext, vector<shared_ptr<StmtNode>> stmtList) {
    if (stmtList.empty()) {
        // if list is empty, the if statement is skipped
        addEdgeToCFG(parent, parentNext);
        return;
    }
    // record the statement number for each statement in the list
    vector<int> stmtNoVector = getStmtNoList(stmtList);
    // add each statement to the cfg
    int size = stmtList.size();
    int previous = parent;
    int next;
    NodeType previousNodeType;
    for (int i = 0; i < size; i++) {
        shared_ptr<StmtNode> stmt = stmtList[i];
        int stmtNumber = stmtNoVector[i];
        if (i < size - 1) {
            next = stmtNoVector[i + 1];
        } else {
            // if this is the last statement in the if statement, it should link to the statement to be
            // executed after the if statement
            next = parentNext;
        }
        if (i > 0 && previousNodeType == AST::IF_NODE) {
            // do not add next for if statement
            previous = invalidStmtNo;
        }
        processStmt(stmt, previous, next);
        previous = stmtNumber;
        previousNodeType = ASTUtils::getNodeType(stmt);
    }
    if (previousNodeType != AST::IF_NODE) {
        addEdgeToCFG(stmtNoVector[size - 1], next);
    }
}
