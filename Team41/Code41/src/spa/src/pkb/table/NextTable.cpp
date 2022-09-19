//
// Created by hongshu wang on 19/9/22.
//

#include "NextTable.h"
#include "../PKBStorage.h"

NextTable::NextTable() {
    header = vector<string>{PKBStorage::NEXT_TABLE_COL1_NAME, PKBStorage::NEXT_TABLE_COL2_NAME};
}
