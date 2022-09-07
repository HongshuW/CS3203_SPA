//
// Created by hongshu wang on 8/9/22.
//

#include "UsesTable.h"
#include "../EntityManager.h"

UsesTable::UsesTable() {
    header = vector<string>{EntityManager::USES_TABLE_COL1_NAME, EntityManager::USES_TABLE_COL2_NAME};
}