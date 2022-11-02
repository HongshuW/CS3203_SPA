//
// Created by hongshu wang on 19/9/22.
//

#include "CallsTable.h"

#include "../PKBStorage.h"

CallsTable::CallsTable() {
  renameHeader(vector<string>{PKBStorage::CALLS_TABLE_COL1_NAME,
                          PKBStorage::CALLS_TABLE_COL2_NAME});
  mapHeader = vector<string>{PKBStorage::STATEMENT_TABLE_COL1_NAME,
                             PKBStorage::CALLS_TABLE_COL2_NAME};
}

void CallsTable::appendRowToSubtables(vector<string> row) {
  // save value 1 and 2 to rows, save value 3 and 2 to stmtNoProcMap
  vector<string> procedureNames = vector<string>{row[0], row[1]};
  vector<string> stmtNoProcPair = vector<string>{row[2], row[1]};
  appendRow(procedureNames);
  stmtNoProcMap.push_back(stmtNoProcPair);
}

shared_ptr<Table> CallsTable::getCallsProcedures() {
  shared_ptr<Table> table = make_shared<Table>(Table(this->getHeader(), this->getRows()));
  return table;
}

shared_ptr<Table> CallsTable::getStmtNoProcMap() {
  shared_ptr<Table> table = make_shared<Table>(Table(mapHeader, stmtNoProcMap));
  return table;
}
