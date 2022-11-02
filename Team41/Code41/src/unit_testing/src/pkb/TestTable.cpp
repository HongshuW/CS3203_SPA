//
// Created by hongshu wang on 31/8/22.
//

#include <list>

#include "catch.hpp"
#include "pkb/table/Table.cpp"

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
    Table variableTable = Table(header, rows);

    // check header is correct
    REQUIRE(variableTable.getHeader()[0] == "variable name");

    // iterate through rows in the table
    REQUIRE(variableTable.getRows()[0] == row1);
    REQUIRE(variableTable.getRows()[1] == row2);
    REQUIRE(variableTable.getRows()[2] == row3);
  }

  SECTION("Get Column by Column Name") {
    vector<string> header{"stmt number", "stmt lhsType"};
    vector<vector<string>> rows;
    vector<string> row1{"1", "assign"};
    vector<string> row2{"2", "while"};
    vector<string> row3{"3", "read"};
    rows.push_back(row1);
    rows.push_back(row2);
    rows.push_back(row3);

    // create and initialise table
    Table stmtTable = Table(header, rows);

    // check statement number column
    vector<string> stmtNoColumn = stmtTable.getColumnByName("stmt number");
    REQUIRE(stmtNoColumn[0] == "1");
    REQUIRE(stmtNoColumn[1] == "2");
    REQUIRE(stmtNoColumn[2] == "3");

    // check statement number column
    vector<string> stmtTypeColumn = stmtTable.getColumnByName("stmt lhsType");
    REQUIRE(stmtTypeColumn[0] == "assign");
    REQUIRE(stmtTypeColumn[1] == "while");
    REQUIRE(stmtTypeColumn[2] == "read");

    // test invalid column
    vector<string> invalidColumn = stmtTable.getColumnByName("dummny column");
    REQUIRE(invalidColumn.size() == 0);
  }

  SECTION("Append rows") {
    vector<string> header{"variable name"};
    vector<vector<string>> rows;
    vector<string> row1{"dummyVar1"};
    vector<string> row2{"dummyVar2"};
    vector<string> row3{"dummyVar3"};

    // create and initialise table
    Table variableTable = Table(header, rows);

    // append rows
    variableTable.appendRow(row1);
    variableTable.appendRow(row2);
    variableTable.appendRow(row3);

    REQUIRE(variableTable.getRows()[0] == row1);
    REQUIRE(variableTable.getRows()[1] == row2);
    REQUIRE(variableTable.getRows()[2] == row3);
  }

  SECTION("Rename header") {
    vector<string> oldHeader{"typo"};
    Table variableTable;
    variableTable.getHeader() = oldHeader;

    vector<string> newHeader{"variable Table"};
    variableTable.renameHeader(newHeader);

    REQUIRE(variableTable.getHeader()[0] == "variable Table");
  }

  SECTION("drop header") {
    Table table =
        Table({"keep", "drop"}, {{"k1", "d1"}, {"k2", "d2"}, {"k3", "d3"}});

    table.dropColFromThis(1);
    Table actual = table;
    Table expected = Table({"keep"}, {{"k1"}, {"k2"}, {"k3"}});

    REQUIRE(actual.isEqual(expected));
  }

  SECTION("drop rows") {
    Table actualTable = Table({"header"}, {{"a"}, {"b"}, {"c"}});
    actualTable.dropRows();

    Table expectedTable = Table();
    expectedTable.renameHeader({"header"});

    REQUIRE(actualTable.isEqual(expectedTable));
  }
}
