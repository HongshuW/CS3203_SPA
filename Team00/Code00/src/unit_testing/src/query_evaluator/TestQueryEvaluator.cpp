//
// Created by Nafour on 31/8/22.
//
#include "catch.hpp"

#include "query_evaluator/QueryEvaluator.h"
#include "query_builder/QueryBuilder.h"

using namespace QB;
using QE::QueryEvaluator;

TEST_CASE("Test query evaluator") {

    auto queryEvaluator = QueryEvaluator(new DataRetriever());
    std::string queryStr = "variable v1; Select v1";
    auto query = QueryBuilder().buildPQLQuery(queryStr);

    std::string queryStr2 = "stmt a, b, c; Select a";
    auto query2 = QueryBuilder().buildPQLQuery(queryStr2);

    SECTION("evaluate variable v1; Select v1") {
        vector<string> header{"variable name"};
        vector<string> variables = vector<string>{"dummyVar1", "dummyVar2", "dummyVar3"};
        list<tuple<string>> rows;
        for (const string& variable: variables) {
            rows.emplace_back(variable);
        }
        Table<string> variableTable;
        variableTable.rows = rows;
        variableTable.header = header;

        QE::QueryResult expected = QE::QueryResult(variableTable);
        QE::QueryResult queryResult = queryEvaluator.evaluate(&query);

        REQUIRE(expected.table.rows.size() == queryResult.table.rows.size());

        auto expectedIt = expected.table.rows.begin();
        auto qRIt = queryResult.table.rows.begin();
        while (expectedIt != expected.table.rows.end()) {
            REQUIRE(get<0>(*expectedIt) == get<0>(*qRIt));
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