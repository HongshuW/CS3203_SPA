//
// Created by hongshu wang on 8/9/22.
//

#ifndef SPA_PROCEDURETABLE_H
#define SPA_PROCEDURETABLE_H

#include "EntityTable.h"
#include <unordered_map>
#include <variant>

typedef variant<string, shared_ptr<unordered_map<int, unordered_set<int>>>> cfgTableRow;

class ProcedureTable : public EntityTable {
public:
    vector<string> cfgSubTableHeader;
    vector<vector<cfgTableRow>> cfgSubTable;
    explicit ProcedureTable();

    // getters
    shared_ptr<Table> getProcedureNames();

    // setters
    void saveCFGofProcedure(string procedure, shared_ptr<unordered_map<int, unordered_set<int>>> cfg);
};


#endif //SPA_PROCEDURETABLE_H
