//
// Created by hongshu wang on 8/9/22.
//

#ifndef SPA_STATEMENTTABLE_H
#define SPA_STATEMENTTABLE_H

#include "EntityTable.h"
class StatementTable : public EntityTable {
 public:
  vector<string> printedVarsHeader;
  vector<string> readVarsHeader;
  vector<vector<string>> printedVars;
  vector<vector<string>> readVars;

  explicit StatementTable();

  // getters
  string getStatementType(string stmtNumber);
  shared_ptr<Table> getStatements();
  shared_ptr<Table> getPrintedVariables();
  shared_ptr<Table> getReadVariables();

  // setters
  void addPrintedVar(vector<string> printStmtVarPair);
  void addReadVar(vector<string> readStmtVarPair);

 private:
  int statementTypeColumn = 1;
};

#endif  // SPA_STATEMENTTABLE_H
