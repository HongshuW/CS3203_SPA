//
// Created by hongshu wang on 19/9/22.
//

#ifndef SPA_CALLSTABLE_H
#define SPA_CALLSTABLE_H

#include "RelationTable.h"

class CallsTable : public RelationTable {
 private:
  vector<string> mapHeader;
  vector<vector<string>> stmtNoProcMap;

 public:
  explicit CallsTable();

  // getters
  shared_ptr<Table> getCallsProcedures();
  shared_ptr<Table> getStmtNoProcMap();

  // setters
  void appendRowToSubtables(vector<string> row);
};

#endif  // SPA_CALLSTABLE_H
