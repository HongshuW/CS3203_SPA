//
// Created by hongshu wang on 8/9/22.
//

#include "ConstantTable.h"

#include "../PKBStorage.h"

ConstantTable::ConstantTable() {
  header = vector<string>{PKBStorage::CONSTANT_TABLE_COL1_NAME};
}
