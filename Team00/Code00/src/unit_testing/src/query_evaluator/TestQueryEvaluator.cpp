//
// Created by Nafour on 31/8/22.
//
#include "catch.hpp"

#include "query_evaluator/QueryEvaluator.h"
#include "query_builder/QueryBuilder.h"

using namespace QB;
using QE::QueryEvaluator;

TEST_CASE("Test query evaluator") {
    PKB pkb = PKB();
    auto queryEvaluator = QueryEvaluator(&pkb);
    std::string queryStr = "variable v1; Select v1";
    auto query = QueryBuilder().buildPQLQuery(queryStr);

    std::string queryStr2 = "stmt a, b, c; Select a";
    auto query2 = QueryBuilder().buildPQLQuery(queryStr2);

    SECTION("test 1") {
        queryEvaluator.evaluate(query);
        //queryEvaluator.evaluate(query2);
//        DesignEntity d = queryEvaluator.getDesignEntity(query2.selectClause->synonym, &query2);
//        getDesignEntityString(d);
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