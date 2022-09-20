//
// Created by Nafour on 1/9/22.
//

#include "QueryResultFormatter.h"
#include <unordered_set>
#include <utility>
#include <iostream>
#include "QueryTupleResult.h"
#include "QueryBooleanResult.h"
#include "QueryResult.h"
#include "TableCombiner.h"

namespace QE {

    vector<string> QueryResultFormatter::formatResult() {

        vector<string> uniqueResultVec;

        if (typeid(queryResult).name() == typeid(QueryBooleanResult).name()) {
            return handleBooleanResult();
        } else {
            return handleTupleResult();
        }


        auto resultsWithDup =  this->queryResult->table.getColumnByName(this->queryResult.colName);

        std::unordered_set<string> uniqueResultSet;
        uniqueResultSet.insert(resultsWithDup.begin(), resultsWithDup.end());

        uniqueResultVec.insert(uniqueResultVec.end(), uniqueResultSet.begin(), uniqueResultSet.end());
        return uniqueResultVec;
    }

    QueryResultFormatter::QueryResultFormatter(shared_ptr<QueryResult> queryResult,
                                               shared_ptr<DataRetriever> dataRetriever):
                                               queryResult(queryResult), dataRetriever(dataRetriever) {

    }

    vector<string> QueryResultFormatter::handleBooleanResult() {
        vector<string> uniqueResultVec;
        shared_ptr<QueryBooleanResult> queryBooleanResult = dynamic_pointer_cast<QueryBooleanResult>(queryResult);
        uniqueResultVec.insert(uniqueResultVec.begin(), queryBooleanResult->boolVal ? "TRUE" : "FALSE");
        return uniqueResultVec;
    }

    vector<string> QueryResultFormatter::handleTupleResult() {
        shared_ptr<QueryTupleResult> queryTupleResult = dynamic_pointer_cast<QueryTupleResult>(queryResult);



        return vector<string>();
    }
} // QE