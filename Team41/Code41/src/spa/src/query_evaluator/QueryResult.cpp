//
// Created by Nafour on 31/8/22.
//

#include "QueryResult.h"

#include <utility>

namespace QE {
    QueryResult::QueryResult(Table table) {
        this->table = std::move(table);
    }

    bool QueryResult::isEmpty() {
        return this->table.rows.empty();
    }

    bool QueryResult::isEqual(QueryResult otherQueryResult) {
        return this->colName == otherQueryResult.colName && this->table.isEqual(otherQueryResult.table);
    }

    QueryResult::QueryResult() = default;
} // QE