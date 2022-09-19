//
// Created by hongshu wang on 19/9/22.
//

#include "CallsTable.h"
#include "../PKBStorage.h"

CallsTable::CallsTable() {
    header = vector<string>{PKBStorage::CALLS_TABLE_COL1_NAME, PKBStorage::CALLS_TABLE_COL2_NAME};
}
