//
// Created by hongshu wang on 8/9/22.
//

#include "ModifiesTable.h"
#include "../EntityManager.h"

ModifiesTable::ModifiesTable() {
    header = vector<string>{EntityManager::MODIFIES_TABLE_COL1_NAME,EntityManager::MODIFIES_TABLE_COL2_NAME};
}