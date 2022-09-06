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

    SECTION ("Test duplicated synonyms with different synonym types") {
        std::string queryStr = "variable s; stmt s; Select s such that Follows* (a, 6)";
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

    SECTION ("Test first arg can be _ in Follows Clause") {
        std::string queryStr = "stmt v; Select v such that Follows (_, v)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_NOTHROW(queryBuilder.buildPQLQuery(queryStr));
    }

    SECTION ("Test first arg can be _ in Parent* Clause") {
        std::string queryStr = "if i; Select i such that Parent* (_, i)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_NOTHROW(queryBuilder.buildPQLQuery(queryStr));
    }

    SECTION ("Test first arg cannot be _ in Modifies Clause") {
        std::string queryStr = "variable v; Select v such that Modifies (_, v)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test first arg cannot be _ in Uses Clause") {
        std::string queryStr = "variable v; Select v such that Uses (_, v)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test wrong first arg RefType in Follows Clause, first arg cannot be Ident") {
        std::string queryStr = "stmt v; Select v such that Follows (\"main\", v)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test wrong first arg RefType in Follows* Clause, first arg cannot be Ident") {
        std::string queryStr = "stmt v; Select v such that Follows* (\"west\", v)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test wrong second arg RefType in Follows Clause, first second cannot be Ident") {
        std::string queryStr = "stmt v; Select v such that Follows (v, \"main\")";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test wrong second arg RefType in Follows* Clause, second arg cannot be Ident") {
        std::string queryStr = "stmt v; Select v such that Follows* (v, \"west\")";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test wrong first arg RefType in Parent Clause, first arg cannot be Ident") {
        std::string queryStr = "if i; Select i such that Parent (\"main\", i)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test wrong first arg RefType in Parent* Clause, first arg cannot be Ident") {
        std::string queryStr = "if i; Select i such that Parent* (\"main\", i)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test wrong second arg RefType in Parent Clause, second arg cannot be Ident") {
        std::string queryStr = "if i; Select i such that Parent (i, \"main\")";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test wrong second arg RefType in Parent* Clause, second arg cannot be Ident") {
        std::string queryStr = "if i; Select i such that Parent* (i, \"main\")";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test wrong second arg RefType in Uses_S Clause (first arg is Integer), second arg cannot be Integer") {
        std::string queryStr = "variable v; Select v such that Uses (1, 2)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test wrong second arg RefType in Uses_S Clause (first arg is non-procedure synonym, "
             "second arg cannot be Integer") {
        std::string queryStr = "variable v; Select v such that Uses (v, 2)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test wrong second arg RefType in Uses_P Clause (first arg is Ident), second arg cannot be Integer") {
        std::string queryStr = "variable v; Select v such that Uses (\"main\", 2)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test wrong second arg RefType in Uses_P Clause (first arg is procedure synonym), "
             "second arg cannot be Integer") {
        std::string queryStr = "procedure p; Select p such that Uses (p, 2)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test wrong second arg RefType in Uses_S Clause (first arg is Integer), second arg cannot be Integer") {
        std::string queryStr = "variable v; Select v such that Uses (1, 2)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test wrong second arg RefType in Uses_S Clause (first arg is non-procedure synonym, "
             "second arg cannot be Integer") {
        std::string queryStr = "variable v; Select v such that Uses (v, 2)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test wrong second arg RefType in Uses_P Clause (first arg is Ident), second arg cannot be Integer") {
        std::string queryStr = "variable v; Select v such that Uses (\"main\", 2)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test wrong second arg RefType in Uses_P Clause (first arg is procedure synonym), "
             "second arg cannot be Integer") {
        std::string queryStr = "procedure p; Select p such that Uses (p, 2)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test wrong first arg synonym type in Follows Clause, first arg cannot be procedure synonym") {
        std::string queryStr = "procedure p; Select p such that Follows (p, _)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test wrong second arg synonym type in Follows Clause, second arg cannot be variable synonym") {
        std::string queryStr = "variable x; if i; Select x such that Follows (i, x)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test wrong first arg synonym type in Follows* Clause, first arg cannot be procedure synonym") {
        std::string queryStr = "procedure p; Select p such that Follows* (p, _)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test wrong first arg synonym type in Follows* Clause, first arg cannot be variable synonym") {
        std::string queryStr = "variable v; Select v such that Follows* (v, _)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test wrong first arg synonym type in Parent Clause, first arg cannot be procedure synonym") {
        std::string queryStr = "procedure p; Select p such that Parent (p, _)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test wrong first arg synonym type in Parent Clause, first arg cannot be variable synonym") {
        std::string queryStr = "variable v; Select v such that Parent (v, _)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test wrong second arg synonym type in Parent* Clause, second arg cannot be procedure synonym") {
        std::string queryStr = "procedure p; if i; Select p such that Parent* (i, p)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test wrong first arg synonym in Uses_S Clause, first arg cannot be a variable synonym") {
        std::string queryStr = "variable v; Select v such that Uses (v, _)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test correct second arg synonym in Uses_S Clause (first arg is non-procedure synonym, "
             "second arg must be variable synonym") {
        std::string queryStr = "assign a; variable v; Select a such that Uses (a, v)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_NOTHROW(queryBuilder.buildPQLQuery(queryStr));
    }

    SECTION ("Test wrong second arg synonym in Uses_S Clause (first arg is non-procedure synonym, "
             "second arg cannot be a procedure synonym") {
        std::string queryStr = "assign a; procedure s; Select a such that Uses (a, s)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test wrong second arg synonym in Uses_S Clause (first arg is non-procedure synonym, "
             "second arg cannot be a stmt synonym") {
        std::string queryStr = "assign a; stmt s; Select a such that Uses (a, s)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test correct second arg synonym in Uses_P Clause (first arg is procedure synonym, "
             "second arg must be variable synonym") {
        std::string queryStr = "procedure p; variable v; Select p such that Uses (p, v)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_NOTHROW(queryBuilder.buildPQLQuery(queryStr));
    }

    SECTION ("Test wrong first arg synonym in Modifies_S Clause, first arg cannot be a variable synonym") {
        std::string queryStr = "variable v; Select v such that Modifies (v, _)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test correct second arg synonym in Modifies_S Clause (first arg is non-procedure synonym, "
             "second arg must be variable synonym") {
        std::string queryStr = "assign a; variable v; Select a such that Modifies (a, v)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_NOTHROW(queryBuilder.buildPQLQuery(queryStr));
    }

    SECTION ("Test wrong second arg synonym in Modifies_S Clause (first arg is non-procedure synonym, "
             "second arg cannot be a procedure synonym") {
        std::string queryStr = "assign a; procedure s; Select a such that Modifies (a, s)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test wrong second arg synonym in Modifies_S Clause (first arg is non-procedure synonym, "
             "second arg cannot be a stmt synonym") {
        std::string queryStr = "assign a; stmt s; Select a such that Modifies (a, s)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }

    SECTION ("Test correct second arg synonym in Modifies_P Clause (first arg is procedure synonym, "
             "second arg must be variable synonym") {
        std::string queryStr = "procedure p; variable v; Select p such that Uses (p, v)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_NOTHROW(queryBuilder.buildPQLQuery(queryStr));
    }

    SECTION ("Test correct second arg synonym in Modifies_P Clause (first arg is procedure synonym, "
             "second arg cannot be a stmt synonym") {
        std::string queryStr = "procedure p; stmt s; Select p such that Uses (p, s)";
        auto queryBuilder = QueryBuilder();
        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLValidationException);
    }
}