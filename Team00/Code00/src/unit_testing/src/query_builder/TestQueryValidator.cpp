//
// Created by Xingchen Lin on 31/8/22.
//

#include "catch.hpp"
#include "query_builder/QueryBuilder.h"
#include "query_builder/Exceptions.h"

using namespace QB;

TEST_CASE ("Test Query Validator") {
    SECTION ("Test duplicated synonyms") {
        std::string queryStr = "variable v1, v1; Select v1";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test undeclared synonym in Select Clause") {
        std::string queryStr = "variable a; Select b";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test undeclared synonym in Such That Clause") {
        std::string queryStr = "stmt s; Select s such that Follows* (a, 6)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }
}