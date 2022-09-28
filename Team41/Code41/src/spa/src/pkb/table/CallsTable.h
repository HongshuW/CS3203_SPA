//
// Created by hongshu wang on 19/9/22.
//

#ifndef SPA_CALLSTABLE_H
#define SPA_CALLSTABLE_H

#include "RelationTable.h"

class CallsTable : public RelationTable {
public:
    vector<string> mapHeader;
    vector<vector<string>> stmtNoProcMap;
    explicit CallsTable();
    void appendRowToSubtables(vector<string> row);
    shared_ptr<Table> getCallsProcedures();
    shared_ptr<Table> getStmtNoProcMap();
};


#endif //SPA_CALLSTABLE_H
