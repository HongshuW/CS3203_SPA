//
// Created by hongshu wang on 8/9/22.
//

#ifndef SPA_STATEMENTTABLE_H
#define SPA_STATEMENTTABLE_H

#include "EntityTable.h"

class StatementTable : public EntityTable {
public:
    explicit StatementTable();
    string getStatementType(string stmtNumber);
};


#endif //SPA_STATEMENTTABLE_H
