//
// Created by hongshu wang on 8/9/22.
//

#include "ConstantTable.h"
#include "../EntityManager.h"

ConstantTable::ConstantTable() {
    header = vector<string>{EntityManager::CONSTANT_TABLE_COL1_NAME};
}
