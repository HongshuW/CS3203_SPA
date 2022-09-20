//
// Created by Nafour on 1/9/22.
//

#include "QueryResultFormatter.h"
#include <unordered_set>
#include <utility>
#include <iostream>

namespace QE {
    QueryResultFormatter::QueryResultFormatter(QueryResult queryResult) {
        this->queryResult = std::move(queryResult);
    }

    vector<string> QueryResultFormatter::formatResult() {
        //todo: remove duplicates in the result
        auto resultsWithDup =  this->queryResult.table.getColumnByName(this->queryResult.colName);
        std::unordered_set<string> uniqueResultSet;
        uniqueResultSet.insert(resultsWithDup.begin(), resultsWithDup.end());
        vector<string> uniqueResultVec;
        uniqueResultVec.insert(uniqueResultVec.end(), uniqueResultSet.begin(), uniqueResultSet.end());
        return uniqueResultVec;
    }

    QueryResultFormatter::QueryResultFormatter(QueryResult queryResult, shared_ptr<DataRetriever> dataRetriever):queryResult(queryResult), dataRetriever(dataRetriever) {

    }
} // QE