//
// Created by Nafour on 1/9/22.
//

#include "QueryResultFormatter.h"

#include <utility>
#include <iostream>

namespace QE {
    QueryResultFormatter::QueryResultFormatter(QueryResult queryResult) {
        this->queryResult = std::move(queryResult);
    }

    vector<string> QueryResultFormatter::formatResult() {
        //todo: remove duplicates in the result
        return this->queryResult.table.getColumnByName(this->queryResult.colName);
    }
} // QE