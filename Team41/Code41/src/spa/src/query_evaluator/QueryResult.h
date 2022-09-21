//
// Created by Nafour on 31/8/22.
//

#ifndef SPA_QUERYRESULT_H
#define SPA_QUERYRESULT_H
#include "pkb/table/Table.h"
#include <string>
#include <vector>

namespace QE {

    class QueryResult {


    public:
        string colName;

        Table table;
        explicit QueryResult(Table table);
        QueryResult();
        bool isEmpty();
        bool isEqual(QueryResult otherQueryResult);

    };

} // QE

#endif //SPA_QUERYRESULT_H
