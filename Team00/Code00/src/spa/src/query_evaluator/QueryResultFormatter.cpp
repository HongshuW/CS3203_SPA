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

    vector<string> QueryResultFormatter::formatResult(const string& colName) {
        auto ans = vector<string>{};
        auto it = std::find(this->queryResult.table.header.begin(), this->queryResult.table.header.end(), colName);
        if (it != this->queryResult.table.header.end())
        {
            auto index = it - this->queryResult.table.header.begin();
            for (auto row: this->queryResult.table.rows) {
                ans.push_back(row[index]);
            }
        }
        return ans;
    }
} // QE