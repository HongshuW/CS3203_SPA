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
        list<tuple<string>> rows;
        tuple<string> row1 = make_tuple("dummyVar1");
        tuple<string> row2 = make_tuple("dummyVar2");
        tuple<string> row3 = make_tuple("dummyVar3");
        rows.push_back(row1);
        rows.push_back(row2);
        rows.push_back(row3);

        // create and initialise table
        Table<string> variableTable;
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
