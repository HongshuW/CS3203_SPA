//
// Created by hongshu wang on 8/9/22.
//

#include "FollowsTable.h"

#include "../PKBStorage.h"

FollowsTable::FollowsTable() {
  renameHeader(vector<string>{PKBStorage::FOLLOWS_TABLE_COL1_NAME,
                              PKBStorage::FOLLOWS_TABLE_COL2_NAME});
}