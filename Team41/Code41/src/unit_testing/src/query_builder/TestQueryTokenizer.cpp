//
// Created by Xingchen Lin on 27/8/22.
//

#include "catch.hpp"
#include "query_builder/QueryTokenizer.h"
#include "query_builder/exceptions/Exceptions.h"

using namespace QB;

TEST_CASE ("Test Query Tokenizer") {
    SECTION ("Test empty query") {
        std::string query = "";

        QueryTokenizer tokenizer = QueryTokenizer(query);
        auto tokens = tokenizer.tokenize();
        auto expected = std::vector<std::string>(
                {});

        REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                           end(expected),
                           [](const std::string l, const std::string o) {
                               return l.compare(o) == 0;
                           }));
    }

    SECTION ("variable v1; Select v1") {
        std::string query = "variable v1; Select v1";

        QueryTokenizer tokenizer = QueryTokenizer(query);
        auto tokens = tokenizer.tokenize();
        auto expected = std::vector<std::string>(
                {"variable", "v1", ";", "Select", "v1"});

        REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                           end(expected),
                           [](const std::string l, const std::string o) {
                               return l.compare(o) == 0;
                           }));
    }

    SECTION ("stmt s; Select s such that Follows (6, s)") {
        std::string query = "stmt s; Select s such that Follows (6, s)";

        QueryTokenizer tokenizer = QueryTokenizer(query);
        auto tokens = tokenizer.tokenize();
        auto expected = std::vector<std::string>(
                {"stmt", "s", ";", "Select", "s", "such", "that", "Follows", "(", "6", ",",
                 "s", ")"});

        REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                           end(expected),
                           [](const std::string l, const std::string o) {
                               return l.compare(o) == 0;
                           }));
    }

    SECTION ("stmt s; Select s such that Follows* (6, s)") {
        std::string query = "stmt s; Select s such that Follows* (6, s)";

        QueryTokenizer tokenizer = QueryTokenizer(query);
        auto tokens = tokenizer.tokenize();
        auto expected = std::vector<std::string>(
                {"stmt", "s", ";", "Select", "s", "such", "that", "Follows*", "(", "6", ",",
                 "s", ")"});

        REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                           end(expected),
                           [](const std::string l, const std::string o) {
                               return l.compare(o) == 0;
                           }));
    }

    SECTION ("stmt s; Select s such that Parent (s, 12)") {
        std::string query = "stmt s; Select s such that Parent (s, 12)";

        QueryTokenizer tokenizer = QueryTokenizer(query);
        auto tokens = tokenizer.tokenize();
        auto expected = std::vector<std::string>(
                {"stmt", "s", ";", "Select", "s", "such", "that", "Parent", "(", "s", ",",
                 "12", ")"});

        REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                           end(expected),
                           [](const std::string l, const std::string o) {
                               return l.compare(o) == 0;
                           }));
    }

    SECTION ("assign a; Select a such that Parent* (_, a)") {
        std::string query = "assign a; Select a such that Parent* (_, a)";

        QueryTokenizer tokenizer = QueryTokenizer(query);
        auto tokens = tokenizer.tokenize();
        auto expected = std::vector<std::string>(
                {"assign", "a", ";", "Select", "a", "such", "that", "Parent*", "(", "_", ",",
                 "a", ")"});

        REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                           end(expected),
                           [](const std::string l, const std::string o) {
                               return l.compare(o) == 0;
                           }));
    }

    SECTION ("variable v; Select v such that Modifies (6, v)") {
        std::string query = "variable v; Select v such that Modifies (6, v)";

        QueryTokenizer tokenizer = QueryTokenizer(query);
        auto tokens = tokenizer.tokenize();
        auto expected = std::vector<std::string>(
                {"variable", "v", ";", "Select", "v", "such", "that", "Modifies", "(", "6", ",",
                 "v", ")"});

        REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                           end(expected),
                           [](const std::string l, const std::string o) {
                               return l.compare(o) == 0;
                           }));
    }

    SECTION ("variable v; Select v such that Uses (6, v)") {
        std::string query = "variable v; Select v such that Uses (6, v)";

        QueryTokenizer tokenizer = QueryTokenizer(query);
        auto tokens = tokenizer.tokenize();
        auto expected = std::vector<std::string>(
                {"variable", "v", ";", "Select", "v", "such", "that", "Uses", "(", "6", ",",
                 "v", ")"});

        REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                           end(expected),
                           [](const std::string l, const std::string o) {
                               return l.compare(o) == 0;
                           }));
    }

    SECTION ("assign a;  Select a pattern a (_, _\"x\"_)") {
        std::string query = "assign a; Select a pattern a (_, _\"X\"_)";

        QueryTokenizer tokenizer = QueryTokenizer(query);
        auto tokens = tokenizer.tokenize();
        auto expected = std::vector<std::string>(
                {"assign", "a", ";", "Select", "a", "pattern", "a", "(", "_", ",", "_",
                 "\"", "X", "\"", "_", ")"});

        REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                           end(expected),
                           [](const std::string l, const std::string o) {
                               return l.compare(o) == 0;
                           }));
    }

    SECTION ("assign a; Select a pattern a (_, _)") {
        std::string query = "assign a; Select a pattern a (_, _)";

        QueryTokenizer tokenizer = QueryTokenizer(query);
        auto tokens = tokenizer.tokenize();
        auto expected = std::vector<std::string>(
                {"assign", "a", ";", "Select", "a", "pattern", "a", "(", "_", ",", "_", ")"});

        REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                           end(expected),
                           [](const std::string l, const std::string o) {
                               return l.compare(o) == 0;
                           }));
    }

    SECTION ("assign a; Select a pattern a (_, _)") {
        std::string query = "assign a; Select a pattern a (_, _)";

        QueryTokenizer tokenizer = QueryTokenizer(query);
        auto tokens = tokenizer.tokenize();
        auto expected = std::vector<std::string>(
                {"assign", "a", ";", "Select", "a", "pattern", "a", "(", "_", ",", "_", ")"});

        REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                           end(expected),
                           [](const std::string l, const std::string o) {
                               return l.compare(o) == 0;
                           }));
    }

    SECTION ("assign a; Select a pattern a (_, _\"x + 1\"_)") {
        std::string query = "assign a; Select a pattern a (_, _\"x + 1\"_)";

        QueryTokenizer tokenizer = QueryTokenizer(query);
        auto tokens = tokenizer.tokenize();
        auto expected = std::vector<std::string>(
                {"assign", "a", ";", "Select", "a", "pattern", "a",
                 "(", "_", ",", "_", "\"", "x + 1", "\"", "_", ")"});

        REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                           end(expected),
                           [](const std::string l, const std::string o) {
                               return l.compare(o) == 0;
                           }));
    }

    SECTION ("assign a; while w; Select w such that Parent* (w, a) pattern a (\"count\", _)") {
        std::string query = "assign a; while w; Select w such that Parent* (w, a) pattern a (\"count\", _)";

        QueryTokenizer tokenizer = QueryTokenizer(query);
        auto tokens = tokenizer.tokenize();
        auto expected = std::vector<std::string>(
                {"assign", "a", ";", "while", "w", ";", "Select", "w", "such", "that", "Parent*", "(",
                 "w", ",", "a", ")", "pattern", "a", "(", "\"", "count", "\"", ",", "_", ")"});

        REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                           end(expected),
                           [](const std::string l, const std::string o) {
                               return l.compare(o) == 0;
                           }));
    }

    SECTION ("assign a; while w; Select w such that Parent* (w, a) pattern a (_\"x\"_, _)") {
        std::string query = "assign a; while w; Select w such that Parent* (w, a) pattern a (_\"x\"_, _)";

        QueryTokenizer tokenizer = QueryTokenizer(query);
        auto tokens = tokenizer.tokenize();
        auto expected = std::vector<std::string>(
                {"assign", "a", ";", "while", "w", ";", "Select", "w", "such", "that", "Parent*", "(",
                 "w", ",", "a", ")", "pattern", "a", "(", "_", "\"", "x", "\"", "_", ",", "_", ")"});

        REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                           end(expected),
                           [](const std::string l, const std::string o) {
                               return l.compare(o) == 0;
                           }));
    }

    SECTION ("assign a; variable v; Select v such that Uses (a, v) pattern a (v, _\"x\"_)") {
        std::string query = "assign a; variable v; Select v such that Uses (a, v) pattern a (v, _\"x\"_)";

        QueryTokenizer tokenizer = QueryTokenizer(query);
        auto tokens = tokenizer.tokenize();
        auto expected = std::vector<std::string>(
                {"assign", "a", ";", "variable", "v", ";", "Select", "v", "such", "that", "Uses", "(",
                 "a", ",", "v", ")", "pattern", "a", "(", "v", ",", "_", "\"", "x", "\"", "_", ")"});

        REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                           end(expected),
                           [](const std::string l, const std::string o) {
                               return l.compare(o) == 0;
                           }));
    }

    SECTION ("assign a; variable v; Select v such that Uses (a, v) pattern a (v, _\"x + y / 3\"_)") {
        std::string query = "assign a; variable v; Select v such that Uses (a, v) pattern a (v, _\"x + y / 3\"_)";

        QueryTokenizer tokenizer = QueryTokenizer(query);
        auto tokens = tokenizer.tokenize();
        auto expected = std::vector<std::string>(
                {"assign", "a", ";", "variable", "v", ";", "Select", "v", "such", "that", "Uses", "(",
                 "a", ",", "v", ")", "pattern", "a", "(", "v", ",", "_", "\"", "x + y / 3", "\"", "_", ")"});

        REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                           end(expected),
                           [](const std::string l, const std::string o) {
                               return l.compare(o) == 0;
                           }));
    }

    SECTION ("Test unexpected tokens") {
        std::string query = "stmt s& Select s such that Follows* (a, 6)";
        QueryTokenizer tokenizer = QueryTokenizer(query);
        REQUIRE_THROWS_AS(tokenizer.tokenize(), PQLTokenizeException);
    }
}

