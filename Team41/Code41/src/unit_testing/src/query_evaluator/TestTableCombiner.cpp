//
// Created by Nafour on 6/9/22.
//
#include "Dummies/TableBuilder.h"
#include "TableComparator.h"
#include "catch.hpp"
#include "pkb/table/Table.h"
#include "query_evaluator/TableCombiner.h"

using namespace QE;
TEST_CASE("Test table combiner") {
  TableCombiner myTableCombiner = TableCombiner();
  TableComparator myTableComparator = TableComparator();
  SECTION("A table joined with an empty table should be itself") {
    string commonHeaderName = "v";
    Table t1 = TestQE::TableBuilder()
                   .setHeaders(vector<string>{"$col1", commonHeaderName})
                   ->addRow({"1", "foo"})
                   ->addRow({"2", "bar"})
                   ->addRow({"3", "baz"})
                   ->addRow({"4", "corge"})
                   ->build();
    Table empty = Table();
    Table actual = *myTableCombiner.hashJoin(make_shared<Table>(t1),
                                             make_shared<Table>(empty));
    Table expected = t1;
    REQUIRE(actual.isEqual(expected));
  }
  SECTION("Test two tables with one col each, and one common column") {
    string commonHeaderName = "v";
    Table t1 = Table();
    Table t2 = Table();
    t1.renameHeader(vector<string>{commonHeaderName});
    auto t1Col1 = vector<string>{"1", "2", "3", "4"};

    t2.renameHeader(vector<string>{commonHeaderName});
    auto t2Col1 = vector<string>{"3", "4", "5", "6"};

    for (int i = 0; i < t1Col1.size(); ++i) {
      t1.appendRow(vector<string>{t1Col1[i]});
      t2.appendRow(vector<string>{t2Col1[i]});
    }
    Table expected = Table();
    expected.renameHeader(vector<string>{commonHeaderName});
    expected.appendRow(vector<string>{"3"});
    expected.appendRow(vector<string>{"4"});

    TableCombiner tableCombiner = TableCombiner();
    Table testResultTable = *myTableCombiner.hashJoin(make_shared<Table>(t1),
                                                      make_shared<Table>(t2));
    TableComparator tableComparator = TableComparator();

    REQUIRE(tableComparator.isEqual(testResultTable, expected) == true);
  }
  SECTION(
      "Test two tables with two cols each, and one common column, with 2 "
      "common row") {
    string commonHeaderName = "v";
    Table t1 = TestQE::TableBuilder()
                   .setHeaders(vector<string>{"$col1", commonHeaderName})
                   ->addRow({"1", "foo"})
                   ->addRow({"2", "bar"})
                   ->addRow({"3", "baz"})
                   ->addRow({"4", "corge"})
                   ->build();
    Table t2 = TestQE::TableBuilder()
                   .setHeaders({"$col2", commonHeaderName})
                   ->addRow({"5", "bar"})
                   ->addRow({"6", "qux"})
                   ->addRow({"7", "baz"})
                   ->addRow({"8", "quux"})
                   ->build();
    Table expected = Table();
    expected.renameHeader(vector<string>{"$col1", commonHeaderName, "$col2"});
    expected.appendRow(vector<string>{"2", "bar", "5"});
    expected.appendRow(vector<string>{"3", "baz", "7"});

    TableCombiner tableCombiner = TableCombiner();
    Table testResultTable = *myTableCombiner.hashJoin(make_shared<Table>(t1),
                                                      make_shared<Table>(t2));
    TableComparator tableComparator = TableComparator();

    REQUIRE(tableComparator.isEqual(testResultTable, expected) == true);
  }
  SECTION(
      "Test two tables with two cols each, and one common column, with no "
      "common row") {
    string commonHeaderName = "v";
    Table t1 = TestQE::TableBuilder()
                   .setHeaders(vector<string>{"$col1", commonHeaderName})
                   ->addRow({"1", "oof"})
                   ->addRow({"2", "rab"})
                   ->addRow({"3", "zab"})
                   ->addRow({"4", "corge"})
                   ->build();
    Table t2 = TestQE::TableBuilder()
                   .setHeaders({"$col2", commonHeaderName})
                   ->addRow({"5", "bar"})
                   ->addRow({"6", "qux"})
                   ->addRow({"7", "baz"})
                   ->addRow({"8", "quux"})
                   ->build();
    Table expected = TestQE::TableBuilder()
                         .setHeaders({"$col1", commonHeaderName, "$col2"})
                         ->build();

    TableCombiner tableCombiner = TableCombiner();
    Table testResultTable = *myTableCombiner.hashJoin(make_shared<Table>(t1),
                                                      make_shared<Table>(t2));
    TableComparator tableComparator = TableComparator();

    REQUIRE(tableComparator.isEqual(testResultTable, expected) == true);
  }
  SECTION(
      "Test two tables with two cols each and two common columns, with two "
      "common row") {
    string commonCol1Name = "a";
    string commonCol2Name = "v";
    Table t1 = Table();
    Table t2 = Table();

    t1.renameHeader(vector<string>{commonCol1Name, commonCol2Name});
    auto t1Col1 = vector<string>{"1", "2", "3", "4"};
    auto t1Col2 = vector<string>{"foo", "bar", "baz", "corge"};

    t2.renameHeader(vector<string>{commonCol1Name, commonCol2Name});
    auto t2Col1 = vector<string>{"2", "3", "4", "5"};
    auto t2Col2 = vector<string>{"bar", "foo", "corge", "baz"};

    for (int i = 0; i < t1Col1.size(); ++i) {
      t1.appendRow(vector<string>{t1Col1[i], t1Col2[i]});
      t2.appendRow(vector<string>{t2Col1[i], t2Col2[i]});
    }
    Table expected = Table();
    expected.renameHeader(vector<string>{commonCol1Name, commonCol2Name});
    expected.appendRow(vector<string>{"2", "bar"});
    expected.appendRow(vector<string>{"4", "corge"});

    TableCombiner tableCombiner = TableCombiner();
    Table testResultTable = *myTableCombiner.hashJoin(make_shared<Table>(t1),
                                                      make_shared<Table>(t2));
    TableComparator tableComparator = TableComparator();

    REQUIRE(tableComparator.isEqual(testResultTable, expected) == true);
  }
  SECTION(
      "Test two tables each with 4 cols, and 2 common columns, with 2 common "
      "row") {
    string commonCol1Name = "a";
    string commonCol2Name = "v";
    Table t1 =
        TestQE::TableBuilder()
            .setHeaders({"$col1", commonCol1Name, "$col2", commonCol2Name})
            ->addRow({"foo", "1", "a", "5"})
            ->addRow({"bar", "2", "b", "6"})
            ->addRow({"baz", "3", "c", "7"})
            ->addRow({"qux", "4", "d", "8"})
            ->build();
    Table t2 =
        TestQE::TableBuilder()
            .setHeaders({"$col1", commonCol1Name, "$col2", commonCol2Name})
            ->addRow({"oof", "2", "e", "3"})
            ->addRow({"rab", "2", "f", "6"})
            ->addRow({"zab", "5", "g", "7"})
            ->addRow({"xuq", "4", "h", "8"})
            ->build();
    Table expected = TestQE::TableBuilder()
                         .setHeaders({"$col1", commonCol1Name, "$col2",
                                      commonCol2Name, "$col1", "$col2"})
                         ->addRow({"bar", "2", "b", "6", "rab", "f"})
                         ->addRow({"qux", "4", "d", "8", "xuq", "h"})
                         ->build();
    Table testResult = *myTableCombiner.hashJoin(make_shared<Table>(t1),
                                                 make_shared<Table>(t2));
    REQUIRE(myTableComparator.isEqual(testResult, expected) == true);
  }
  SECTION(
      "Test two tables each with 4 cols, and 2 common columns, with no common "
      "row") {
    string commonCol1Name = "a";
    string commonCol2Name = "v";
    Table t1 =
        TestQE::TableBuilder()
            .setHeaders({"$col1", commonCol1Name, "$col2", commonCol2Name})
            ->addRow({"foo", "1", "a", "5"})
            ->addRow({"bar", "3", "b", "6"})
            ->addRow({"baz", "3", "c", "7"})
            ->addRow({"qux", "4", "d", "8"})
            ->build();
    Table t2 =
        TestQE::TableBuilder()
            .setHeaders({"$col1", commonCol1Name, "$col2", commonCol2Name})
            ->addRow({"oof", "2", "e", "3"})
            ->addRow({"rab", "2", "f", "6"})
            ->addRow({"zab", "5", "g", "7"})
            ->addRow({"xuq", "6", "h", "8"})
            ->build();
    Table expected = TestQE::TableBuilder()
                         .setHeaders({"$col1", commonCol1Name, "$col2",
                                      commonCol2Name, "$col1", "$col2"})
                         ->build();
    Table testResult = *myTableCombiner.hashJoin(make_shared<Table>(t1),
                                                 make_shared<Table>(t2));
    REQUIRE(myTableComparator.isEqual(testResult, expected) == true);
  }
  SECTION("A table joined with an empty table should be itself") {
    string commonHeaderName = "v";
    Table t1 = TestQE::TableBuilder()
                   .setHeaders(vector<string>{"$col1", commonHeaderName})
                   ->addRow({"1", "foo"})
                   ->addRow({"2", "bar"})
                   ->addRow({"3", "baz"})
                   ->addRow({"4", "corge"})
                   ->build();
    Table empty = Table();
    Table actual = *myTableCombiner.hashJoin(make_shared<Table>(t1),
                                             make_shared<Table>(empty));
    Table expected = t1;
    REQUIRE(actual.isEqual(expected));
  }
  SECTION("Test two tables with one col each, and one common column") {
    string commonHeaderName = "v";
    Table t1 = Table();
    Table t2 = Table();
    t1.renameHeader(vector<string>{commonHeaderName});
    auto t1Col1 = vector<string>{"1", "2", "3", "4"};

    t2.renameHeader(vector<string>{commonHeaderName});
    auto t2Col1 = vector<string>{"3", "4", "5", "6"};

    for (int i = 0; i < t1Col1.size(); ++i) {
      t1.appendRow(vector<string>{t1Col1[i]});
      t2.appendRow(vector<string>{t2Col1[i]});
    }
    Table expected = Table();
    expected.renameHeader(vector<string>{commonHeaderName});
    expected.appendRow(vector<string>{"3"});
    expected.appendRow(vector<string>{"4"});

    TableCombiner tableCombiner = TableCombiner();
    Table testResultTable = *myTableCombiner.hashJoin(make_shared<Table>(t1),
                                                      make_shared<Table>(t2));
    TableComparator tableComparator = TableComparator();

    REQUIRE(tableComparator.isEqual(testResultTable, expected) == true);
  }

  SECTION(
      "Test two tables with two cols each, and one common column, with 2 "
      "common row") {
    string commonHeaderName = "v";
    Table t1 = TestQE::TableBuilder()
                   .setHeaders(vector<string>{"$col1", commonHeaderName})
                   ->addRow({"1", "foo"})
                   ->addRow({"2", "bar"})
                   ->addRow({"3", "baz"})
                   ->addRow({"4", "corge"})
                   ->build();
    Table t2 = TestQE::TableBuilder()
                   .setHeaders({"$col2", commonHeaderName})
                   ->addRow({"5", "bar"})
                   ->addRow({"6", "qux"})
                   ->addRow({"7", "baz"})
                   ->addRow({"8", "quux"})
                   ->build();
    Table expected = Table();
    expected.renameHeader(vector<string>{"$col1", commonHeaderName, "$col2"});
    expected.appendRow(vector<string>{"2", "bar", "5"});
    expected.appendRow(vector<string>{"3", "baz", "7"});

    TableCombiner tableCombiner = TableCombiner();
    Table testResultTable = *myTableCombiner.hashJoin(make_shared<Table>(t1),
                                                      make_shared<Table>(t2));
    TableComparator tableComparator = TableComparator();

    REQUIRE(tableComparator.isEqual(testResultTable, expected) == true);
  }
  SECTION(
      "Test two tables with two cols each, and one common column, with no "
      "common row") {
    string commonHeaderName = "v";
    Table t1 = TestQE::TableBuilder()
                   .setHeaders(vector<string>{"$col1", commonHeaderName})
                   ->addRow({"1", "oof"})
                   ->addRow({"2", "rab"})
                   ->addRow({"3", "zab"})
                   ->addRow({"4", "corge"})
                   ->build();
    Table t2 = TestQE::TableBuilder()
                   .setHeaders({"$col2", commonHeaderName})
                   ->addRow({"5", "bar"})
                   ->addRow({"6", "qux"})
                   ->addRow({"7", "baz"})
                   ->addRow({"8", "quux"})
                   ->build();
    Table expected = TestQE::TableBuilder()
                         .setHeaders({"$col1", commonHeaderName, "$col2"})
                         ->build();

    TableCombiner tableCombiner = TableCombiner();
    Table testResultTable = *myTableCombiner.hashJoin(make_shared<Table>(t1),
                                                      make_shared<Table>(t2));
    TableComparator tableComparator = TableComparator();

    REQUIRE(tableComparator.isEqual(testResultTable, expected) == true);
  }
  SECTION(
      "Test two tables with two cols each and two common columns, with two "
      "common row") {
    string commonCol1Name = "a";
    string commonCol2Name = "v";
    Table t1 = Table();
    Table t2 = Table();

    t1.renameHeader(vector<string>{commonCol1Name, commonCol2Name});
    auto t1Col1 = vector<string>{"1", "2", "3", "4"};
    auto t1Col2 = vector<string>{"foo", "bar", "baz", "corge"};

    t2.renameHeader(vector<string>{commonCol1Name, commonCol2Name});
    auto t2Col1 = vector<string>{"2", "3", "4", "5"};
    auto t2Col2 = vector<string>{"bar", "foo", "corge", "baz"};

    for (int i = 0; i < t1Col1.size(); ++i) {
      t1.appendRow(vector<string>{t1Col1[i], t1Col2[i]});
      t2.appendRow(vector<string>{t2Col1[i], t2Col2[i]});
    }
    Table expected = Table();
    expected.renameHeader(vector<string>{commonCol1Name, commonCol2Name});
    expected.appendRow(vector<string>{"2", "bar"});
    expected.appendRow(vector<string>{"4", "corge"});

    TableCombiner tableCombiner = TableCombiner();
    Table testResultTable = *myTableCombiner.hashJoin(make_shared<Table>(t1),
                                                      make_shared<Table>(t2));
    TableComparator tableComparator = TableComparator();

    REQUIRE(tableComparator.isEqual(testResultTable, expected) == true);
  }
  SECTION(
      "Test two tables each with 4 cols, and 2 common columns, with 2 common "
      "row") {
    string commonCol1Name = "a";
    string commonCol2Name = "v";
    Table t1 =
        TestQE::TableBuilder()
            .setHeaders({"$col1", commonCol1Name, "$col2", commonCol2Name})
            ->addRow({"foo", "1", "a", "5"})
            ->addRow({"bar", "2", "b", "6"})
            ->addRow({"baz", "3", "c", "7"})
            ->addRow({"qux", "4", "d", "8"})
            ->build();
    Table t2 =
        TestQE::TableBuilder()
            .setHeaders({"$col1", commonCol1Name, "$col2", commonCol2Name})
            ->addRow({"oof", "2", "e", "3"})
            ->addRow({"rab", "2", "f", "6"})
            ->addRow({"zab", "5", "g", "7"})
            ->addRow({"xuq", "4", "h", "8"})
            ->build();
    Table expected = TestQE::TableBuilder()
                         .setHeaders({"$col1", commonCol1Name, "$col2",
                                      commonCol2Name, "$col1", "$col2"})
                         ->addRow({"bar", "2", "b", "6", "rab", "f"})
                         ->addRow({"qux", "4", "d", "8", "xuq", "h"})
                         ->build();
    Table testResult = *myTableCombiner.hashJoin(make_shared<Table>(t1),
                                                 make_shared<Table>(t2));
    REQUIRE(myTableComparator.isEqual(testResult, expected) == true);
  }
  SECTION(
      "Test two tables each with 4 cols, and 2 common columns, with no common "
      "row") {
    string commonCol1Name = "a";
    string commonCol2Name = "v";
    Table t1 =
        TestQE::TableBuilder()
            .setHeaders({"$col1", commonCol1Name, "$col2", commonCol2Name})
            ->addRow({"foo", "1", "a", "5"})
            ->addRow({"bar", "3", "b", "6"})
            ->addRow({"baz", "3", "c", "7"})
            ->addRow({"qux", "4", "d", "8"})
            ->build();
    Table t2 =
        TestQE::TableBuilder()
            .setHeaders({"$col1", commonCol1Name, "$col2", commonCol2Name})
            ->addRow({"oof", "2", "e", "3"})
            ->addRow({"rab", "2", "f", "6"})
            ->addRow({"zab", "5", "g", "7"})
            ->addRow({"xuq", "6", "h", "8"})
            ->build();
    Table expected = TestQE::TableBuilder()
                         .setHeaders({"$col1", commonCol1Name, "$col2",
                                      commonCol2Name, "$col1", "$col2"})
                         ->build();
    Table testResult = *myTableCombiner.hashJoin(make_shared<Table>(t1),
                                                 make_shared<Table>(t2));
    REQUIRE(myTableComparator.isEqual(testResult, expected) == true);
  }
}
