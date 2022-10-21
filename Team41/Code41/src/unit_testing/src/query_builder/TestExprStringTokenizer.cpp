//
// Created by Xingchen Lin on 13/9/22.
//

#include "catch.hpp"
#include "query_builder/commons/ExprStringTokenizer.h"
#include "query_builder/exceptions/Exceptions.h"

using namespace QB;

TEST_CASE("Test ExprString Tokenizer") {
  SECTION("Test empty exprStr") {
    string exprStr = "";

    ExprStringTokenizer tokenizer = ExprStringTokenizer(exprStr);
    auto tokens = tokenizer.tokenize();
    auto expected = vector<string>({});

    REQUIRE(equal(begin(tokens), end(tokens), begin(expected), end(expected),
                  [](const std::string l, const std::string o) {
                    return l.compare(o) == 0;
                  }));
  }

  SECTION("x") {
    string exprStr = "x";

    ExprStringTokenizer tokenizer = ExprStringTokenizer(exprStr);
    auto tokens = tokenizer.tokenize();
    auto expected = vector<string>({"x"});

    REQUIRE(equal(begin(tokens), end(tokens), begin(expected), end(expected),
                  [](const std::string l, const std::string o) {
                    return l.compare(o) == 0;
                  }));
  }

  SECTION("x + 1") {
    string exprStr = "x + 1";

    ExprStringTokenizer tokenizer = ExprStringTokenizer(exprStr);
    auto tokens = tokenizer.tokenize();
    auto expected = vector<string>({"x", "+", "1"});

    REQUIRE(equal(begin(tokens), end(tokens), begin(expected), end(expected),
                  [](const std::string l, const std::string o) {
                    return l.compare(o) == 0;
                  }));
  }

  SECTION("x+2*y/z") {
    string exprStr = "x+2*y/z";

    ExprStringTokenizer tokenizer = ExprStringTokenizer(exprStr);
    auto tokens = tokenizer.tokenize();
    auto expected = vector<string>({"x", "+", "2", "*", "y", "/", "z"});

    REQUIRE(equal(begin(tokens), end(tokens), begin(expected), end(expected),
                  [](const std::string l, const std::string o) {
                    return l.compare(o) == 0;
                  }));
  }

  SECTION("(x+2)*y/z") {
    string exprStr = "(x+2)*y/z";

    ExprStringTokenizer tokenizer = ExprStringTokenizer(exprStr);
    auto tokens = tokenizer.tokenize();
    auto expected =
        vector<string>({"(", "x", "+", "2", ")", "*", "y", "/", "z"});

    REQUIRE(equal(begin(tokens), end(tokens), begin(expected), end(expected),
                  [](const std::string l, const std::string o) {
                    return l.compare(o) == 0;
                  }));
  }

  SECTION("test123 * 7") {
    string exprStr = "test123 * 7";

    ExprStringTokenizer tokenizer = ExprStringTokenizer(exprStr);
    auto tokens = tokenizer.tokenize();
    auto expected = vector<string>({"test123", "*", "7"});

    REQUIRE(equal(begin(tokens), end(tokens), begin(expected), end(expected),
                  [](const std::string l, const std::string o) {
                    return l.compare(o) == 0;
                  }));
  }

  SECTION("(test123 * x)") {
    string exprStr = "(test123 * x)";

    ExprStringTokenizer tokenizer = ExprStringTokenizer(exprStr);
    auto tokens = tokenizer.tokenize();
    auto expected = vector<string>({"(", "test123", "*", "x", ")"});

    REQUIRE(equal(begin(tokens), end(tokens), begin(expected), end(expected),
                  [](const std::string l, const std::string o) {
                    return l.compare(o) == 0;
                  }));
  }

  SECTION("Unexpected token: 123Test * 7; Throw PQLParseException") {
    string exprStr = "123Test * 7";
    ExprStringTokenizer tokenizer = ExprStringTokenizer(exprStr);
    REQUIRE_THROWS_AS(tokenizer.tokenize(), PQLParseException);
  }

  SECTION("Unexpected token: x & 1; Throw PQLParseException") {
    string exprStr = "x & 1";
    ExprStringTokenizer tokenizer = ExprStringTokenizer(exprStr);
    REQUIRE_THROWS_AS(tokenizer.tokenize(), PQLParseException);
  }

  SECTION("Invalid parentheses: (x + 1(; Throw PQLParseException") {
    string exprStr = "(x + 1(";
    ExprStringTokenizer tokenizer = ExprStringTokenizer(exprStr);
    REQUIRE_THROWS_AS(tokenizer.tokenize(), PQLParseException);
  }
}
