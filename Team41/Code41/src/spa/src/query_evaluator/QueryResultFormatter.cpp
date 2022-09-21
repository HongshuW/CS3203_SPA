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
    }

    QueryResultFormatter::QueryResultFormatter(shared_ptr<QueryResult> queryResult1,
                                               shared_ptr<DataRetriever> dataRetriever):
                                               queryResult1(queryResult1), dataRetriever(dataRetriever) {

    }

    QueryResultFormatter::QueryResultFormatter(QueryResult queryResult):
            queryResult(queryResult) {

    }

    vector<string> QueryResultFormatter::handleBooleanResult() {
        vector<string> uniqueResultVec;
        shared_ptr<QueryBooleanResult> queryBooleanResult = dynamic_pointer_cast<QueryBooleanResult>(queryResult1);
        uniqueResultVec.insert(uniqueResultVec.begin(), queryBooleanResult->boolVal ? "TRUE" : "FALSE");
        return uniqueResultVec;
    }

    vector<string> QueryResultFormatter::handleTupleResult() {
        shared_ptr<QueryTupleResult> queryTupleResult = dynamic_pointer_cast<QueryTupleResult>(queryResult1);

        for(auto elem: *queryTupleResult->returnTuple) {
            int index = elem.index();
            int SYNONYM_INDEX = 0;
            int ATTR_REF_INDEX = 1;
            if (index == SYNONYM_INDEX) {
                Synonym synonym = std::get<Synonym>(elem);
                if (!queryTupleResult->table.hasCol(synonym.synonym)) {
                    TableCombiner tableCombiner = TableCombiner();

                }
            }
        }


        return vector<string>();
    }
} // QE