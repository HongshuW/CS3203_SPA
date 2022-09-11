//
// Created by hongshu wang on 8/9/22.
//

#include "ParentTable.h"
#include "../EntityManager.h"

ParentTable::ParentTable() {
    header = vector<string>{EntityManager::PARENT_TABLE_COL1_NAME, EntityManager::PARENT_TABLE_COL2_NAME};
}