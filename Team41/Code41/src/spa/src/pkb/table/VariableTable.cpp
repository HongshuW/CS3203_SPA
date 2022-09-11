//
// Created by hongshu wang on 8/9/22.
//

#include "VariableTable.h"
#include "../EntityManager.h"

VariableTable::VariableTable() {
    header = vector<string>{EntityManager::VARIABLE_TABLE_COL1_NAME};
}