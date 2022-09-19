//
// Created by hongshu wang on 19/9/22.
//

#include "AffectsTable.h"
#include "../PKBStorage.h"

AffectsTable::AffectsTable() {
    header = vector<string>{PKBStorage::AFFECTS_TABLE_COL1_NAME, PKBStorage::AFFECTS_TABLE_COL2_NAME};
}
