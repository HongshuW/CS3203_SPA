//
// Created by hongshu wang on 29/9/22.
//

#ifndef SPA_CFG_H
#define SPA_CFG_H

#include <unordered_map>
#include <unordered_set>

#include "./AST/ProcedureNode.h"

using namespace std;
typedef unordered_map<int, unordered_set<int>> cfgType;

class CFG {
 public:
  shared_ptr<cfgType> cfg;

  explicit CFG(
      ProcedureNode procedureNode,
      shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers);

 private:
  shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers;
  int invalidStmtNo = 0;

  vector<int> getStmtNoList(vector<shared_ptr<StmtNode>> stmtList);
  void processStmtList(vector<shared_ptr<StmtNode>> stmtList);
  void processStmt(shared_ptr<StmtNode> stmt, int previous, int next);
  void addEdgeToCFG(int current, int next);
  void processWhileStmtList(int parent, vector<shared_ptr<StmtNode>> stmtList);
  void processIfStmtList(int parent, int parentNext,
                         vector<shared_ptr<StmtNode>> stmtList);
};

#endif  // SPA_CFG_H
