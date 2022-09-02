//
// Created by Nafour on 1/9/22.
//
#include <iostream>
#include <vector>
#include "catch.hpp"
#include "query_evaluator/QueryResultFormatter.h"

using namespace QE;
TEST_CASE("Test query result formatter") {
    vector<string> header{"variable name"};
    vector<string> variables = vector<string>{"dummyVar1", "dummyVar2", "dummyVar3"};
    list<tuple<string>> rows;
    for (const string& variable: variables) {
        rows.emplace_back(variable);
    }
    Table<string> variableTable;
    variableTable.rows = rows;
    variableTable.header = header;
    QueryResult queryResult = QueryResult(variableTable);

    SECTION("test select col from query result with variable table and format result") {
        QueryResultFormatter queryResultFormatter = QueryResultFormatter(queryResult);
        vector<string> ans = queryResultFormatter.formatResult("variable name");
        REQUIRE(ans.size() == queryResult.table.rows.size());
        int i = 0;
        for (auto& tup : variableTable.rows) {
            REQUIRE(ans[i] == get<0>(tup));
            i++;
        }

    }
}