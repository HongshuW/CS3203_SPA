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
        shared_ptr<QueryResult> queryResult1;
        shared_ptr<DataRetriever> dataRetriever;
        vector<string> handleBooleanResult();
        vector<string> handleTupleResult();

    public:
        explicit QueryResultFormatter(QueryResult);
        explicit QueryResultFormatter(shared_ptr<QueryResult> queryResult1, shared_ptr<DataRetriever> dataRetriever);
        vector<string> formatResult();
    };

} // QE

#endif //SPA_QUERYRESULTFORMATTER_H
