//
// Created by hongshu wang on 8/9/22.
//

#include "UsesTable.h"

#include "../PKBStorage.h"

UsesTable::UsesTable() {
  header = vector<string>{PKBStorage::USES_TABLE_COL1_NAME,
                          PKBStorage::USES_TABLE_COL2_NAME};
}