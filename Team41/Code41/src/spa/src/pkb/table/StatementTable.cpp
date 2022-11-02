//
// Created by hongshu wang on 8/9/22.
//

#include "StatementTable.h"

#include "../PKBStorage.h"
#include "../exception/KeyNotFoundException.h"

StatementTable::StatementTable() {
  renameHeader(vector<string>{PKBStorage::STATEMENT_TABLE_COL1_NAME,
                              PKBStorage::STATEMENT_TABLE_COL2_NAME});
  printedVarsHeader = vector<string>{PKBStorage::STATEMENT_TABLE_COL1_NAME,
                                     PKBStorage::VARIABLE_TABLE_COL1_NAME};
  readVarsHeader = vector<string>{PKBStorage::STATEMENT_TABLE_COL1_NAME,
                                  PKBStorage::VARIABLE_TABLE_COL1_NAME};
}

string StatementTable::getStatementType(string stmtNumber) {
  vector<string> row = getRowByPrimaryKey(stmtNumber);
  if (row.empty()) {
    throw new KeyNotFoundException(getHeader()[primaryKeyColumnIndex],
                                   stmtNumber);
  }
  return row[statementTypeColumn];
}

shared_ptr<Table> StatementTable::getStatements() {
  shared_ptr<Table> table =
      make_shared<Table>(Table(this->getHeader(), this->getRows()));
  return table;
}

shared_ptr<Table> StatementTable::getPrintedVariables() {
  shared_ptr<Table> table = make_shared<Table>(Table());
  table->renameHeader(printedVarsHeader);
  table->setRows(printedVars);
  return table;
}

shared_ptr<Table> StatementTable::getReadVariables() {
  shared_ptr<Table> table = make_shared<Table>(Table());
  table->renameHeader(readVarsHeader);
  table->setRows(readVars);
  return table;
}

void StatementTable::addPrintedVar(vector<string> printStmtVarPair) {
  printedVars.push_back(printStmtVarPair);
}

void StatementTable::addReadVar(vector<string> readStmtVarPair) {
  readVars.push_back(readStmtVarPair);
}
