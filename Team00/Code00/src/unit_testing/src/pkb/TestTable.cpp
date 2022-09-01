//
// Created by hongshu wang on 31/8/22.
//

#include "catch.hpp"
#include <list>
#include "pkb/Table.cpp"

using namespace std;

TEST_CASE("Test Table") {
    SECTION("Create Table") {
        vector<string> header{"variable name"};
        list<vector<string>> rows;
        vector<string> row1{"dummyVar1"};
        vector<string> row2{"dummyVar2"};
        vector<string> row3{"dummyVar3"};
        rows.push_back(row1);
        rows.push_back(row2);
        rows.push_back(row3);

        // create and initialise table
        Table variableTable;
        variableTable.header = header;
        variableTable.rows = rows;

        // check header is correct
        REQUIRE(variableTable.header[0] == "variable name");

        // iterate through rows in the table
        auto rowsIterator = variableTable.rows.begin();
        REQUIRE(*rowsIterator == row1);
        advance(rowsIterator, 1);
        REQUIRE(*rowsIterator == row2);
        advance(rowsIterator, 1);
        REQUIRE(*rowsIterator == row3);
    }
}
