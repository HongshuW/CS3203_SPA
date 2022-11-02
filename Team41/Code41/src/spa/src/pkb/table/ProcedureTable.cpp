//
// Created by hongshu wang on 8/9/22.
//

#include "ProcedureTable.h"

#include "../PKBStorage.h"

ProcedureTable::ProcedureTable() {
    renameHeader(vector<string>{PKBStorage::PROCEDURE_TABLE_COL1_NAME});
  cfgSubTableHeader = vector<string>{PKBStorage::PROCEDURE_TABLE_COL1_NAME,
                                     PKBStorage::PROCEDURE_TABLE_COL2_NAME};
  cfgSubTable = vector<vector<cfgTableRow>>();
}

shared_ptr<Table> ProcedureTable::getProcedureNames() {
  shared_ptr<Table> table = make_shared<Table>(Table(this->getHeader(), this->getRows()));
  return table;
}

void ProcedureTable::saveCFGofProcedure(
    string procedure, shared_ptr<unordered_map<int, unordered_set<int>>> cfg) {
  vector<cfgTableRow> row = vector<cfgTableRow>{procedure, cfg};
  cfgSubTable.push_back(row);
}
