//
// Created by hongshu wang on 8/9/22.
//

#include "../exception/KeyNotFoundException.h"
#include "StatementTable.h"

StatementTable::StatementTable() {
    header = vector<string>{"$statement_number", "$statement_type"};
}

string StatementTable::getStatementType(string stmtNumber) {
    vector<string> row = getRowByPrimaryKey(stmtNumber);
    if (row.empty()) {
        throw new KeyNotFoundException(header[0], stmtNumber);
    }
    return row[1];
}
