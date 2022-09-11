//
// Created by hongshu wang on 8/9/22.
//

#include "FollowsTable.h"
#include "../EntityManager.h"

FollowsTable::FollowsTable() {
    header = vector<string>{EntityManager::FOLLOWS_TABLE_COL1_NAME, EntityManager::FOLLOWS_TABLE_COL2_NAME};
}