//
// Created by Nafour on 31/8/22.
//
#include "catch.hpp"

#include "query_evaluator/QueryEvaluator.h"
#include "query_builder/QueryBuilder.h"

using namespace QB;
using QE::QueryEvaluator;
using namespace std;

TEST_CASE("Test query evaluator") {

    auto queryEvaluator = QueryEvaluator(new DataRetriever());
    string queryStr = "variable v1; Select v1";
    auto query = QueryBuilder().buildPQLQuery(queryStr);

    string queryStr2 = "stmt a, b, c; Select a";
    auto query2 = QueryBuilder().buildPQLQuery(queryStr2);

    SECTION("evaluate variable v1; Select v1") {
        vector<string> header{"variable name"};
        vector<string> variables = vector<string>{"dummyVar1", "dummyVar2", "dummyVar3"};
        vector<vector<string>> rows;
        for (const string& variable: variables) {
            rows.push_back(vector<string>{variable});
        }
        Table variableTable;
        variableTable.rows = rows;
        variableTable.header = header;

        QE::QueryResult expected = QE::QueryResult(variableTable);
        QE::QueryResult queryResult = queryEvaluator.evaluate(&query);

        REQUIRE(expected.table.rows.size() == queryResult.table.rows.size());

        auto expectedIt = expected.table.rows.begin();
        auto qRIt = queryResult.table.rows.begin();
        while (expectedIt != expected.table.rows.end()) {
            REQUIRE((*expectedIt)[0] == (*qRIt)[0]);
            qRIt++;
            expectedIt++;
        }
    }
    SECTION("Test getDesignEntity variable v1; Select v1") {
        DesignEntity d = queryEvaluator.getDesignEntity(query.selectClause->synonym, &query);
        REQUIRE(d == DesignEntity::VARIABLE);

    }
    SECTION("Test getDesignEntity stmt a, b, c; Select a") {
        DesignEntity d = queryEvaluator.getDesignEntity(query2.selectClause->synonym, &query2);
        REQUIRE(d == DesignEntity::STMT);
    }
}