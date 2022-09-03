//
// Created by Nafour on 1/9/22.
//

#ifndef SPA_QUERYRESULTFORMATTER_H
#define SPA_QUERYRESULTFORMATTER_H

#include "QueryResult.h"

namespace QE {

    class QueryResultFormatter {
        QueryResult queryResult;
    public:
        explicit QueryResultFormatter(QueryResult queryResult);
        vector<string> formatResult(const string& colName);
    };

} // QE

#endif //SPA_QUERYRESULTFORMATTER_H
