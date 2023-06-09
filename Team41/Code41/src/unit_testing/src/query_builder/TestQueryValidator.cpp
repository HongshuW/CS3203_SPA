//
// Created by Xingchen Lin on 31/8/22.
//

#include "catch.hpp"
#include "query_builder/QueryBuilder.h"
#include "query_builder/exceptions/Exceptions.h"

using namespace QB;

TEST_CASE("Test Query Validator") {
  SECTION("Test duplicated synonyms") {
    std::string queryStr = "variable v1, v1; Select v1";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION("Test duplicated synonyms with different synonym types") {
    std::string queryStr =
        "variable s; stmt s; Select s such that Follows* (a, 6)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION("Test undeclared synonym in Select Clause") {
    std::string queryStr = "variable a; Select b";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION("Test undeclared synonym in Such That Clause") {
    std::string queryStr = "stmt s; Select s such that Follows* (a, 6)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION("Test first arg can be _ in Follows Clause") {
    std::string queryStr = "stmt v; Select v such that Follows (_, v)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_NOTHROW(queryBuilder.buildPQLQuery(queryStr));
  }

  SECTION("Test first arg can be _ in Parent* Clause") {
    std::string queryStr = "if i; Select i such that Parent* (_, i)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_NOTHROW(queryBuilder.buildPQLQuery(queryStr));
  }

  SECTION("Test first arg cannot be _ in Modifies Clause") {
    std::string queryStr = "variable v; Select v such that Modifies (_, v)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION("Test first arg cannot be _ in Uses Clause") {
    std::string queryStr = "variable v; Select v such that Uses (_, v)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION(
      "Test wrong first arg synonym lhsType in Follows Clause, first arg "
      "cannot be procedure synonym") {
    std::string queryStr = "procedure p; Select p such that Follows (p, _)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION(
      "Test wrong second arg synonym lhsType in Follows Clause, second arg "
      "cannot be variable synonym") {
    std::string queryStr =
        "variable x; if i; Select x such that Follows (i, x)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION(
      "Test wrong first arg synonym lhsType in Follows* Clause, first arg "
      "cannot be procedure synonym") {
    std::string queryStr = "procedure p; Select p such that Follows* (p, _)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION(
      "Test wrong first arg synonym lhsType in Follows* Clause, first arg "
      "cannot be variable synonym") {
    std::string queryStr = "variable v; Select v such that Follows* (v, _)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION(
      "Test wrong first arg synonym lhsType in Parent Clause, first arg cannot "
      "be procedure synonym") {
    std::string queryStr = "procedure p; Select p such that Parent (p, _)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION(
      "Test wrong first arg synonym lhsType in Parent Clause, first arg cannot "
      "be variable synonym") {
    std::string queryStr = "variable v; Select v such that Parent (v, _)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION(
      "Test wrong second arg synonym lhsType in Parent* Clause, second arg "
      "cannot be procedure synonym") {
    std::string queryStr =
        "procedure p; if i; Select p such that Parent* (i, p)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION(
      "Test wrong first arg synonym in Uses_S Clause, first arg cannot be a "
      "variable synonym") {
    std::string queryStr = "variable v; Select v such that Uses (v, _)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION(
      "Test correct second arg synonym in Uses_S Clause (first arg is "
      "non-procedure synonym, "
      "second arg must be variable synonym") {
    std::string queryStr =
        "assign a; variable v; Select a such that Uses (a, v)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_NOTHROW(queryBuilder.buildPQLQuery(queryStr));
  }

  SECTION(
      "Test wrong second arg synonym in Uses_S Clause (first arg is "
      "non-procedure synonym, "
      "second arg cannot be a procedure synonym") {
    std::string queryStr =
        "assign a; procedure s; Select a such that Uses (a, s)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION(
      "Test wrong second arg synonym in Uses_S Clause (first arg is "
      "non-procedure synonym, "
      "second arg cannot be a stmt synonym") {
    std::string queryStr = "assign a; stmt s; Select a such that Uses (a, s)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION(
      "Test correct second arg synonym in Uses_P Clause (first arg is "
      "procedure synonym, "
      "second arg must be variable synonym") {
    std::string queryStr =
        "procedure p; variable v; Select p such that Uses (p, v)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_NOTHROW(queryBuilder.buildPQLQuery(queryStr));
  }

  SECTION(
      "Test wrong first arg synonym in Modifies_S Clause, first arg cannot be "
      "a variable synonym") {
    std::string queryStr = "variable v; Select v such that Modifies (v, _)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION(
      "Test correct second arg synonym in Modifies_S Clause (first arg is "
      "non-procedure synonym, "
      "second arg must be variable synonym") {
    std::string queryStr =
        "assign a; variable v; Select a such that Modifies (a, v)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_NOTHROW(queryBuilder.buildPQLQuery(queryStr));
  }

  SECTION(
      "Test wrong second arg synonym in Modifies_S Clause (first arg is "
      "non-procedure synonym, "
      "second arg cannot be a procedure synonym") {
    std::string queryStr =
        "assign a; procedure s; Select a such that Modifies (a, s)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION(
      "Test wrong second arg synonym in Modifies_S Clause (first arg is "
      "non-procedure synonym, "
      "second arg cannot be a stmt synonym") {
    std::string queryStr =
        "assign a; stmt s; Select a such that Modifies (a, s)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION(
      "Test correct second arg synonym in Modifies_P Clause (first arg is "
      "procedure synonym, "
      "second arg must be variable synonym") {
    std::string queryStr =
        "procedure p; variable v; Select p such that Uses (p, v)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_NOTHROW(queryBuilder.buildPQLQuery(queryStr));
  }

  SECTION(
      "Test correct second arg synonym in Modifies_P Clause (first arg is "
      "procedure synonym, "
      "second arg cannot be a stmt synonym") {
    std::string queryStr =
        "procedure p; stmt s; Select p such that Uses (p, s)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION("Test undeclared synonym in Pattern Clause") {
    std::string queryStr = "variable a; Select a pattern b (_, _)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION("Test invalid design entity type of argument 2 in Pattern Clause") {
    std::string queryStr = "assign a; constant c; Select a pattern a (c, _)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION("Test invalid Design Entity type of argument 2 in Pattern Clause") {
    // Validate if agr2 is a synonym, it must be declared as variable
    std::string queryStr =
        "assign a; procedure s; while w; Select a such that Modifies (a, s) "
        "pattern a (w, _\"x\"_)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION("Test correct args RefType in Calls Clause") {
    std::string queryStr =
        "procedure p; Select p such that Calls (\"main\", p)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_NOTHROW(queryBuilder.buildPQLQuery(queryStr));
  }

  SECTION("Test correct args RefType in Next Clause") {
    std::string queryStr = "assign a; Select a such that Next (a, 3)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_NOTHROW(queryBuilder.buildPQLQuery(queryStr));
  }

  SECTION("Test correct args RefType in Affects* Clause") {
    std::string queryStr = "assign a; Select a such that Next (a, _)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_NOTHROW(queryBuilder.buildPQLQuery(queryStr));
  }

  SECTION(
      "Test invalid Design Entity lhsType of argument 1 in Calls Clause, first "
      "arg must be procedure") {
    std::string queryStr =
        "procedure p; assign a; Select p such that Calls* (a, p)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION(
      "Test invalid Design Entity lhsType of argument 1 in Affects* Clause, "
      "first arg cannot be procedure") {
    std::string queryStr =
        "procedure p; assign a; Select p such that Affects* (p, a)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION(
      "Test invalid Design Entity lhsType of argument 2 in Next* Clause, "
      "second arg cannot be variable") {
    std::string queryStr =
        "variable v; assign a; Select p such that Next* (a, v)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION("Test invalid Design Entity AttrRef pair in Select Clause") {
    std::string queryStr = "assign a; Select <a, v.procName> pattern a (_, _)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION("Test undeclared AttrRef in Select Clause") {
    std::string queryStr = "assign a; Select <a, v.value> pattern a (_, _)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION("Test undeclared synonym in Pattern Clause") {
    std::string queryStr = "assign a; Select a pattern a (_, _) and w (_, _)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION("Test synonym in AttrRef not declared in Select Clause") {
    std::string queryStr =
        "assign a1, a2; Select <a1.procName, a2> such that Affects (a1, a2)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION("Test incorrect DesignEntity and AttrName pair in With Clause") {
    std::string queryStr =
        "assign a1, a2; Select <a1, a2> such that Affects (a1, a2) with "
        "a1.value = 2";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION("Test synonym in AttrRef not declared in With Clause") {
    std::string queryStr = "procedure p; Select p with x.value = 2";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION("Test different WithRef for lhs and rhs in With Clause") {
    std::string queryStr = "procedure p; Select p with p.procName = 2";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION(
      "Invalid Design Entity type of argument 1 in Pattern Clause, throw "
      "PQLValidationException") {
    std::string queryStr = "variable a; Select a pattern a (_, _)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION("Invalid Design Entity in CallsT clause") {
    std::string queryStr = "variable v; Select v such that Calls* (v, _)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION(
      "Invalid Design Entity type of argument 1 in Pattern Clause, throw "
      "PQLValidationException") {
    std::string queryStr = "read a; Select a pattern a (\"x\", _)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION(
      "Invalid Design Entity in Affects clause, should be 2 assign synonyms") {
    std::string queryStr = "variable v; Select v such that Affects (v, _)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION(
      "Invalid Design Entity in AffectsT clause, should be 2 stmt synonyms") {
    std::string queryStr =
        "procedure s1, s2; Select <s1, s2> such that Affects* (s1, s2)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION(
      "Valid Design Entity in Affects clause, not exception will be thrown") {
    std::string queryStr =
        "stmt s1, s2; Select <s1, s2> such that Affects (s1, s2)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_NOTHROW(queryBuilder.buildPQLQuery(queryStr));
  }

  SECTION(
      "Valid Design Entity in AffectsT clause, not exception will be thrown") {
    std::string queryStr =
        "print s1; read s2; Select <s1, s2> such that Affects* (s1, s2)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_NOTHROW(queryBuilder.buildPQLQuery(queryStr));
  }

  SECTION("Invalid match of design entity and AttrName in Select clause") {
    std::string queryStr =
        "constant c; if i; Select <i, c.stmt#> with i.stmt# = c.value";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION("Valid assign pattern but invalid design entity type") {
    std::string queryStr = "if i; Select i pattern i (\"test\", _)";
    auto queryBuilder = QueryBuilder();

    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION("Valid assign pattern but invalid design entity type") {
    std::string queryStr = "while w; Select w pattern w(\"test\", \"x + 1\")";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION("Valid assign/while pattern but invalid design entity type") {
    std::string queryStr = "stmt w; Select w pattern w(_, _)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION("Valid assign/while pattern but invalid design entity type") {
    std::string queryStr = "if i; Select i pattern i(_, _)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION("Valid if pattern but invalid design entity type") {
    std::string queryStr = "while w; Select w pattern w(_, _, _)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }

  SECTION("Valid if pattern") {
    std::string queryStr = "if i; Select i pattern i(_, _, _)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_NOTHROW(queryBuilder.buildPQLQuery(queryStr));
  }

  SECTION("Valid assign pattern - any match") {
    std::string queryStr = "assign a; Select a pattern a(_, _)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_NOTHROW(queryBuilder.buildPQLQuery(queryStr));
  }

  SECTION("Valid assign pattern - full match") {
    std::string queryStr = "assign a; Select a pattern a (_, \"x + 1\")";
    auto queryBuilder = QueryBuilder();
    REQUIRE_NOTHROW(queryBuilder.buildPQLQuery(queryStr));
  }

  SECTION("Valid assign pattern - partial match") {
    std::string queryStr = "assign a; Select a pattern a (_, _\"x + 1 - y\"_)";
    auto queryBuilder = QueryBuilder();
    REQUIRE_NOTHROW(queryBuilder.buildPQLQuery(queryStr));
  }

  SECTION("Calls can be treated as synonym - Semantic Error") {
    std::string queryStr =
        "stmt s; procedure p; assign a; Select s pattern a(_, _\"x\"_) and "
        "Calls(p, \"Helper\")";
    auto queryBuilder = QueryBuilder();
    REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr),
                      PQLValidationException);
  }
}