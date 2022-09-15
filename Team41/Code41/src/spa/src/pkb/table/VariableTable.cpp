//
// Created by hongshu wang on 8/9/22.
//

#include "VariableTable.h"
#include "../PKBStorage.h"

VariableTable::VariableTable() {
    header = vector<string>{PKBStorage::VARIABLE_TABLE_COL1_NAME};
}