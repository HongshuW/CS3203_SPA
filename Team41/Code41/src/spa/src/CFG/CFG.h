//
// Created by hongshu wang on 29/9/22.
//

#ifndef SPA_CFG_H
#define SPA_CFG_H

#include "./AST/ProcedureNode.h"
#include <unordered_map>
#include <unordered_set>

using namespace std;

class CFG {
public:
    shared_ptr<unordered_map<int, unordered_set<int>>> cfg;

    explicit CFG(ProcedureNode procedureNode, shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers);

private:
    shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers;

    void processStmtList(vector<shared_ptr<StmtNode>> stmtList);
    void processStmt(shared_ptr<StmtNode> stmt, int stmtNumber, int previous, int next);
    void addEdgeToCFG(int current, int next);
    void processWhileStmtList(int parent, vector<shared_ptr<StmtNode>> stmtList);
    void processIfStmtList(int parent, int parentNext, vector<shared_ptr<StmtNode>> stmtList);
};


#endif //SPA_CFG_H
