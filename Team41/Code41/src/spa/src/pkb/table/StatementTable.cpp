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

string StatementTable::getStatementType(const string& stmtNumber) {
  vector<string> row = getRowByPrimaryKey(stmtNumber);
  if (row.empty()) {
    throw KeyNotFoundException(getHeader()[primaryKeyColumnIndex], stmtNumber);
  }
  return row[statementTypeColumn];
}

shared_ptr<Table> StatementTable::getStatements() {
  shared_ptr<Table> table =
      make_shared<Table>(Table(this->getHeader(), this->getRows()));
  return table;
}

shared_ptr<Table> StatementTable::getPrintedVariables() const {
  shared_ptr<Table> table = make_shared<Table>(Table());
  table->renameHeader(printedVarsHeader);
  table->setRows(printedVars);
  return table;
}

shared_ptr<Table> StatementTable::getReadVariables() const {
  shared_ptr<Table> table = make_shared<Table>(Table());
  table->renameHeader(readVarsHeader);
  table->setRows(readVars);
  return table;
}

void StatementTable::addPrintedVar(const vector<string>& printStmtVarPair) {
  printedVars.push_back(printStmtVarPair);
}

void StatementTable::addReadVar(const vector<string>& readStmtVarPair) {
  readVars.push_back(readStmtVarPair);
}
