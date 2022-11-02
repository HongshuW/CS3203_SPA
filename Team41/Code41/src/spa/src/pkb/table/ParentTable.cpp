//
// Created by hongshu wang on 8/9/22.
//

#include "ParentTable.h"

#include "../PKBStorage.h"

ParentTable::ParentTable() {
  renameHeader(vector<string>{PKBStorage::PARENT_TABLE_COL1_NAME,
                          PKBStorage::PARENT_TABLE_COL2_NAME});
}