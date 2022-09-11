//
// Created by hongshu wang on 8/9/22.
//

#include "ProcedureTable.h"
#include "../EntityManager.h"

ProcedureTable::ProcedureTable() {
    header = vector<string>{EntityManager::PROCEDURE_TABLE_COL1_NAME};
}
