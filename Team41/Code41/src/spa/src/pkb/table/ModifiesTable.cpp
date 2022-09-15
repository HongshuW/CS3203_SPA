//
// Created by hongshu wang on 8/9/22.
//

#include "ModifiesTable.h"
#include "../PKBStorage.h"

ModifiesTable::ModifiesTable() {
    header = vector<string>{PKBStorage::MODIFIES_TABLE_COL1_NAME,PKBStorage::MODIFIES_TABLE_COL2_NAME};
}