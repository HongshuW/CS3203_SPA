//
// Created by Xingchen Lin on 11/9/22.
//

#include "catch.hpp"
#include "parser/Parser.h"
#include "parser/SPExceptions.h"
#include "parser/Tokenizer.h"

using namespace SourceParser;
using namespace std;

TEST_CASE("Stress Test CondExprNode Parsing in Parser") {
  SECTION("x == 1") {
    vector<string> tokens =
        vector<string>({"procedure", "testProcedure", "{", "while", "(", "x",
                        "==", "1", ")", "{", "read", "num1", ";", "}", "}"});
    Parser parser = Parser(tokens);
    REQUIRE_NOTHROW(parser.parse());
  }

  SECTION("x != y") {
    vector<string> tokens =
        vector<string>({"procedure", "testProcedure", "{", "while", "(", "x",
                        "!=", "y", ")", "{", "read", "num1", ";", "}", "}"});
    Parser parser = Parser(tokens);
    REQUIRE_NOTHROW(parser.parse());
  }

  SECTION("x <= y") {
    vector<string> tokens =
        vector<string>({"procedure", "testProcedure", "{", "while", "(", "x",
                        "<=", "y", ")", "{", "read", "num1", ";", "}", "}"});
    Parser parser = Parser(tokens);
    REQUIRE_NOTHROW(parser.parse());
  }

  SECTION("!(x == y)") {
    vector<string> tokens = vector<string>(
        {"procedure", "testProcedure", "{", "while", "(", "!", "(", "x",
         "==", "y", ")", ")", "{", "read", "num1", ";", "}", "}"});
    Parser parser = Parser(tokens);
    REQUIRE_NOTHROW(parser.parse());
  }

  SECTION("!(x + 1 <= y)") {
    vector<string> tokens = vector<string>({"procedure", "testProcedure",
                                            "{",         "while",
                                            "(",         "!",
                                            "(",         "x",
                                            "+",         "1",
                                            "<=",        "y",
                                            ")",         ")",
                                            "{",         "read",
                                            "num1",      ";",
                                            "}",         "}"});
    Parser parser = Parser(tokens);
    REQUIRE_NOTHROW(parser.parse());
  }

  SECTION("(x != y) && (z > 2)") {
    vector<string> tokens = vector<string>({"procedure", "testProcedure",
                                            "{",         "while",
                                            "(",         "(",
                                            "x",         "!=",
                                            "y",         ")",
                                            "&&",        "(",
                                            "z",         ">",
                                            "2",         ")",
                                            ")",         "{",
                                            "read",      "num1",
                                            ";",         "}",
                                            "}"});
    Parser parser = Parser(tokens);
    REQUIRE_NOTHROW(parser.parse());
  }

  SECTION("((x == y) || (x <= 3)) && (z > 2)") {
    vector<string> tokens = vector<string>({"procedure", "testProcedure",
                                            "{",         "while",
                                            "(",         "(",
                                            "(",         "x",
                                            "==",        "y",
                                            ")",         "||",
                                            "(",         "x",
                                            "<=",        "3",
                                            ")",         ")",
                                            "&&",        "(",
                                            "z",         ">",
                                            "2",         ")",
                                            ")",         "{",
                                            "read",      "num1",
                                            ";",         "}",
                                            "}"});
    Parser parser = Parser(tokens);
    REQUIRE_NOTHROW(parser.parse());
  }

  SECTION("(x + 1 == y) && (z > 2)") {
    vector<string> tokens = vector<string>({"procedure", "testProcedure",
                                            "{",         "while",
                                            "(",         "(",
                                            "x",         "+",
                                            "1",         "==",
                                            "y",         ")",
                                            "&&",        "(",
                                            "z",         ">",
                                            "2",         ")",
                                            ")",         "{",
                                            "read",      "num1",
                                            ";",         "}",
                                            "}"});
    Parser parser = Parser(tokens);
    REQUIRE_NOTHROW(parser.parse());
  }

  SECTION("((a == b) && (!(c % 2 == 0))) || (d < e)") {
    vector<string> tokens = vector<string>({"procedure", "testProcedure",
                                            "{",         "while",
                                            "(",         "(",
                                            "(",         "a",
                                            "==",        "b",
                                            ")",         "&&",
                                            "(",         "!",
                                            "(",         "c",
                                            "%",         "2",
                                            "==",        "0",
                                            ")",         ")",
                                            ")",         "||",
                                            "(",         "d",
                                            "<",         "e",
                                            ")",         ")",
                                            "{",         "read",
                                            "num1",      ";",
                                            "}",         "}"});
    Parser parser = Parser(tokens);
    REQUIRE_NOTHROW(parser.parse());
  }

  SECTION("(4 / x - y) != 2") {
    vector<string> tokens = vector<string>({"procedure", "testProcedure",
                                            "{",         "while",
                                            "(",         "(",
                                            "4",         "/",
                                            "x",         "-",
                                            "y",         ")",
                                            "!=",        "2",
                                            ")",         "{",
                                            "read",      "num1",
                                            ";",         "}",
                                            "}"});
    Parser parser = Parser(tokens);
    REQUIRE_NOTHROW(parser.parse());
  }

  SECTION("a == (b + c)") {
    vector<string> tokens = vector<string>(
        {"procedure", "testProcedure", "{", "while", "(", "a", "==", "(", "b",
         "+", "c", ")", ")", "{", "read", "num1", ";", "}", "}"});
    Parser parser = Parser(tokens);
    REQUIRE_NOTHROW(parser.parse());
  }

  SECTION("(a) != (((b)) + (c))") {
    vector<string> tokens = vector<string>({"procedure", "testProcedure",
                                            "{",         "while",
                                            "(",         "(",
                                            "a",         ")",
                                            "!=",        "(",
                                            "(",         "(",
                                            "b",         ")",
                                            ")",         "+",
                                            "(",         "c",
                                            ")",         ")",
                                            ")",         "{",
                                            "read",      "num1",
                                            ";",         "}",
                                            "}"});
    Parser parser = Parser(tokens);
    REQUIRE_NOTHROW(parser.parse());
  }

  SECTION("(x % 2) != 0") {
    vector<string> tokens = vector<string>(
        {"procedure", "testProcedure", "{", "while", "(", "(", "x", "%", "2",
         ")", "!=", "0", ")", "{", "read", "num1", ";", "}", "}"});
    Parser parser = Parser(tokens);
    REQUIRE_NOTHROW(parser.parse());
  }

  SECTION("(x <= 1 - (y - z)) || ((!(x == 2)) && (x != y + z))") {
    string source =
        "procedure testProcedure {"
        "while ((x <= 1 - (y - z)) || ((!(x == 2)) && (x != y + z))) {x=1;}"
        "}";

    Tokenizer tokenizer = Tokenizer(source);
    auto tokens = tokenizer.tokenize();
    Parser parser = Parser(tokens);
    REQUIRE_NOTHROW(parser.parse());
  }

  //! Test for invalid inputs
  SECTION("(x == y && (z > 2)") {
    vector<string> tokens = vector<string>({"procedure", "testProcedure",
                                            "{",         "while",
                                            "(",         "(",
                                            "x",         "==",
                                            "y",         "&&",
                                            "(",         "z",
                                            ">",         "2",
                                            ")",         ")",
                                            "{",         "read",
                                            "num1",      ";",
                                            "}",         "}"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("(x == y) $$ (z > 2)") {
    vector<string> tokens = vector<string>({"procedure", "testProcedure",
                                            "{",         "while",
                                            "(",         "(",
                                            "x",         "==",
                                            "y",         ")",
                                            "$$",        "(",
                                            "z",         ">",
                                            "2",         ")",
                                            ")",         "{",
                                            "read",      "num1",
                                            ";",         "}",
                                            "}"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("x $$ 1") {
    vector<string> tokens = vector<string>({"x", "&&", "1"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("(x &= y) || (z > 2)") {
    vector<string> tokens = vector<string>(
        {"(", "x", "&=", "y", ")", "||", "(", "z", ">", "2", ")"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("(x == y || (z > 2)") {
    vector<string> tokens =
        vector<string>({"(", "x", "==", "y", "||", "(", "z", ">", "2", ")"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("(x == y) || (z ^& 2)") {
    vector<string> tokens =
        vector<string>({"(", "x", "==", "y", "||", "(", "z", "^&", "2", ")"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("(x == y || z > 2)") {
    vector<string> tokens =
        vector<string>({"(", "x", "==", "y", "||", "z", ">", "2", ")"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }
}