//
// Created by Tan Wei En on 2/9/22.
//

#include "catch.hpp"
#include "iostream"
#include "parser/Tokenizer.h"

using namespace SourceParser;
using SourceParser::Tokenizer;
using namespace std;

TEST_CASE ("Test Tokenizer") {
    SECTION ("test empty source program") {
        std::string source = "";

    Tokenizer tokenizer = Tokenizer(source);
    auto tokens = tokenizer.tokenize();
    auto expected = std::vector<std::string>(
            {});

    REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                       end(expected),
                       [](const std::string l, const std::string o) {
                           return l.compare(o) == 0;
                       }));
    }

    SECTION ("test source program with one empty procedure") {
        std::string source = "procedure {"
                             "}";

        Tokenizer tokenizer = Tokenizer(source);
        auto tokens = tokenizer.tokenize();
        auto expected = std::vector<std::string>(
                {"procedure", "{", "}"});

        REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                           end(expected),
                           [](const std::string l, const std::string o) {
                               return l.compare(o) == 0;
                           }));
    }

    SECTION ("test simple source program") {
        std::string source = "procedure testProcedure {"
                                "x = 1;"
                             "}";

        Tokenizer tokenizer = Tokenizer(source);
        auto tokens = tokenizer.tokenize();

        auto expected = std::vector<std::string>(
                {"procedure", "testProcedure", "{", "x", "=", "1", ";", "}"});

        REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                           end(expected),
                           [](const std::string l, const std::string o) {
                               return l.compare(o) == 0;
                           }));
    }

    SECTION ("test source program with multiple procedures") {
        std::string source = "procedure firstProcedure {"
                             "x = 1;"
                             "}"
                             "procedure secondProcedure {"
                             "y = 2;"
                             "}";

        Tokenizer tokenizer = Tokenizer(source);
        auto tokens = tokenizer.tokenize();

        auto expected = std::vector<std::string>(
                {"procedure", "firstProcedure", "{", "x", "=", "1", ";", "}", "procedure", "secondProcedure", "{", "y", "=", "2", ";", "}"});

        REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                           end(expected),
                           [](const std::string l, const std::string o) {
                               return l.compare(o) == 0;
                           }));
    }
}