//
// Created by Nafour on 6/9/22.
//
#include "catch.hpp"
#include "pkb/Table.h"
#include "query_evaluator/TableCombiner.h"
#include "TableComparator.h"

using namespace QE;
TEST_CASE("Test table combiner") {
    SECTION("Test two tables with one col each, and one common column") {
        string commonHeaderName = "v";
        Table t1 = Table();
        Table t2 = Table();
        t1.header = vector<string>{ commonHeaderName};
        auto t1Col1 = vector<string>{"1","2","3","4"};

        t2.header = vector<string>{ commonHeaderName};
        auto t2Col1 = vector<string>{"3","4","5","6"};

        for (int i = 0; i < t1Col1.size(); ++i) {
            t1.rows.push_back(vector<string>{t1Col1[i]});
            t2.rows.push_back(vector<string>{t2Col1[i]});
        }
        Table expected = Table();
        expected.header = vector<string>{ commonHeaderName};
        expected.rows.push_back(vector<string>{"3"});
        expected.rows.push_back(vector<string>{"4"});

        TableCombiner tableCombiner = TableCombiner();
        Table testResultTable = tableCombiner.joinTable(t1, t2);
        TableComparator tableComparator = TableComparator();

        REQUIRE(tableComparator.isEqual(testResultTable, expected) == true);
    }
    SECTION("Test two tables with two cols each, and one common column") {
        string commonHeaderName = "v";
        Table t1 = Table();
        Table t2 = Table();
        t1.header = vector<string>{"$col1", commonHeaderName};
        auto t1Col1 = vector<string>{"1","2","3","4"};
        auto t1Col2 = vector<string>{"foo", "bar", "baz", "corge"};

        t2.header = vector<string>{"$col2", commonHeaderName};
        auto t2Col1 = vector<string>{"5","6","7","8"};
        auto t2Col2 = vector<string>{ "bar", "qux", "baz", "quux"};

        for (int i = 0; i < t1Col1.size(); ++i) {
            t1.rows.push_back(vector<string>{t1Col1[i], t1Col2[i]});
            t2.rows.push_back(vector<string>{t2Col1[i], t2Col2[i]});
        }
        Table expected = Table();
        expected.header = vector<string>{"$col1", commonHeaderName, "$col2"};
        expected.rows.push_back(vector<string>{"2", "bar", "5"});
        expected.rows.push_back(vector<string>{"3", "baz", "7"});

        TableCombiner tableCombiner = TableCombiner();
        Table testResultTable = tableCombiner.joinTable(t1, t2);
        TableComparator tableComparator = TableComparator();

        REQUIRE(tableComparator.isEqual(testResultTable, expected) == true);
    }
}
