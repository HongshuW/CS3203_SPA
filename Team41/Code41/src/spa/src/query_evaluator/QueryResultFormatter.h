//
// Created by Nafour on 1/9/22.
//

#ifndef SPA_QUERYRESULTFORMATTER_H
#define SPA_QUERYRESULTFORMATTER_H

#include "QueryResult.h"
#include "pkb/DataRetriever.h"
namespace QE {

    class QueryResultFormatter {
        QueryResult queryResult;
        shared_ptr<DataRetriever> dataRetriever;
    public:
        explicit QueryResultFormatter(QueryResult queryResult);
        explicit QueryResultFormatter(QueryResult queryResult, shared_ptr<DataRetriever> dataRetriever);
        vector<string> formatResult();
    };

} // QE

#endif //SPA_QUERYRESULTFORMATTER_H
