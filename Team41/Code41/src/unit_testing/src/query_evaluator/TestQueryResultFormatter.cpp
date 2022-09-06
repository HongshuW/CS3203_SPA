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
    vector<string> variables = vector<string>{"dummyVarA", "dummyVarB"};
    vector<vector<string>> rows;
    for (const string& variable: variables) {
        rows.push_back(vector<string>{variable});
    }
    Table expectedTable;
    expectedTable.rows = rows;
    expectedTable.header = header;
    QueryResult queryResult = QueryResult(expectedTable);
    queryResult.colName = "variable name";

    SECTION("test select col from query result with variable table and format result") {
        QueryResultFormatter queryResultFormatter = QueryResultFormatter(queryResult);
        vector<string> ans = queryResultFormatter.formatResult();
        auto expected = expectedTable.getColumnByName("variable name");
        REQUIRE(ans.size() == queryResult.table.rows.size());

        int i = 0;
        for (const string& expectedVarName : expected) {
            REQUIRE(ans[i] == expectedVarName);
            i++;
        }

    }
}