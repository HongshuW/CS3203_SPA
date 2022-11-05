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
  string getStatementType(const string& stmtNumber);
  shared_ptr<Table> getStatements();
  [[nodiscard]] shared_ptr<Table> getPrintedVariables() const;
  [[nodiscard]] shared_ptr<Table> getReadVariables() const;

  // setters
  void addPrintedVar(const vector<string>& printStmtVarPair);
  void addReadVar(const vector<string>& readStmtVarPair);

 private:
  int statementTypeColumn = 1;
};

#endif  // SPA_STATEMENTTABLE_H
