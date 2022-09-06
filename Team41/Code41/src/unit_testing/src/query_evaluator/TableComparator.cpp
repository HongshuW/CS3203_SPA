//
// Created by Nafour on 6/9/22.
//

#include "TableComparator.h"

bool TableComparator::isEqual(Table testResultTable, Table expected) {
    if (testResultTable.header.size() != expected.header.size()) return false;
    if (testResultTable.rows.size() != expected.rows.size()) return false;
    for (int i = 0; i < expected.header.size(); ++i) {
        if (expected.header[i] != testResultTable.header[i]) return false;
    }
    for (int i = 0; i < expected.rows.size(); ++i) {
        auto expectedRow = expected.rows[i];
        auto testResultRow = testResultTable.rows[i];
        if (expectedRow.size() != testResultRow.size()) return false;
        for (int j = 0; j < expectedRow.size(); ++j) {
            if (expectedRow[j] != testResultRow[j]) return false;
        }

    }
    return true;
}
