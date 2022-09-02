//
// Created by Nafour on 31/8/22.
//

#include "QueryResult.h"

#include <utility>

namespace QE {
    QueryResult::QueryResult(Table<string> table) {
        this->table = table;
    }

    bool QueryResult::isEmpty() {
        return this->table.rows.empty();
    }

    QueryResult::QueryResult() = default;
} // QE