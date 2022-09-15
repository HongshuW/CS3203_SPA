//
// Created by hongshu wang on 8/9/22.
//

#include "../exception/KeyNotFoundException.h"
#include "StatementTable.h"
#include "../PKBStorage.h"

StatementTable::StatementTable() {
    header = vector<string>{PKBStorage::STATEMENT_TABLE_COL1_NAME, PKBStorage::STATEMENT_TABLE_COL2_NAME};
}

string StatementTable::getStatementType(string stmtNumber) {
    vector<string> row = getRowByPrimaryKey(stmtNumber);
    if (row.empty()) {
        throw new KeyNotFoundException(header[0], stmtNumber);
    }
    return row[1];
}
