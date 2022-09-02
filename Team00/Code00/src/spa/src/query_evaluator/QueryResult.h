//
// Created by Nafour on 31/8/22.
//

#ifndef SPA_QUERYRESULT_H
#define SPA_QUERYRESULT_H
#include "pkb/Table.h"
#include <string>
#include <vector>

namespace QE {

    class QueryResult {

    public:
        Table<string> table;
        explicit QueryResult(Table<string> table);
        QueryResult();
        bool isEmpty();

    };

} // QE

#endif //SPA_QUERYRESULT_H
