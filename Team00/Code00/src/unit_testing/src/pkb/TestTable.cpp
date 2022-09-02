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
        vector<vector<string>> rows;
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
        REQUIRE(variableTable.rows[0] == row1);
        REQUIRE(variableTable.rows[1] == row2);
        REQUIRE(variableTable.rows[2] == row3);
    }

    SECTION("Get Column by Column Name") {
        vector<string> header{"stmt number", "stmt type"};
        vector<vector<string>> rows;
        vector<string> row1{"1", "assign"};
        vector<string> row2{"2", "while"};
        vector<string> row3{"3", "read"};
        rows.push_back(row1);
        rows.push_back(row2);
        rows.push_back(row3);

        // create and initialise table
        Table stmtTable;
        stmtTable.header = header;
        stmtTable.rows = rows;

        // check statement number column
        vector<string> stmtNoColumn = stmtTable.getColumnByName("stmt number");
        REQUIRE(stmtNoColumn[0] == "1");
        REQUIRE(stmtNoColumn[1] == "2");
        REQUIRE(stmtNoColumn[2] == "3");

        // check statement number column
        vector<string> stmtTypeColumn = stmtTable.getColumnByName("stmt type");
        REQUIRE(stmtTypeColumn[0] == "assign");
        REQUIRE(stmtTypeColumn[1] == "while");
        REQUIRE(stmtTypeColumn[2] == "read");

        // test invalid column
        vector<string> invalidColumn = stmtTable.getColumnByName("dummny column");
        REQUIRE(invalidColumn.size() == 0);
    }
}
