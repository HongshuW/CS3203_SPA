//
// Created by hongshu wang on 8/9/22.
//

#include "ProcedureTable.h"
#include "../PKBStorage.h"

ProcedureTable::ProcedureTable() {
    header = vector<string>{PKBStorage::PROCEDURE_TABLE_COL1_NAME};
}
