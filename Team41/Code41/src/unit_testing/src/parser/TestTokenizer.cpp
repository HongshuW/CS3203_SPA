//
// Created by Tan Wei En on 2/9/22.
//

#include "catch.hpp"
#include "iostream"
#include "parser/Tokenizer.h"
#include "parser/Exceptions.h"

using namespace SourceParser;
using SourceParser::Tokenizer;
using namespace std;

// todo move source program test to a .txt file
// todo test errors thrown
// todo test && and ||

TEST_CASE ("Test Tokenizer") {
    SECTION ("test basic source program with one statement") {
        std::string source = "procedure testProcedure {"
                             "x = 1;"
                             "}";

        Tokenizer tokenizer = Tokenizer(source);
        auto tokens = tokenizer.tokenize();

//        for(int i = 0; i < tokens.size(); i++)
//            std::cout << tokens[i] << ' ';

        auto expected = std::vector<std::string>(
                {"procedure", "testProcedure", "{", "x", "=", "1", ";", "}"});

        REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                           end(expected),
                           [](const std::string l, const std::string o) {
                               return l.compare(o) == 0;
                           }));
    }

    SECTION ("test basic source program with multiple statements") {
        std::string source = "procedure testProcedure {"
                             "x = 1;"
                             "Print x;"
                             "Read x;"
                             "Call secondProcedure;"
                             "}";

        Tokenizer tokenizer = Tokenizer(source);
        auto tokens = tokenizer.tokenize();

//        for(int i = 0; i < tokens.size(); i++)
//            std::cout << tokens[i] << ' ';

        auto expected = std::vector<std::string>(
                {"procedure", "testProcedure", "{", "x", "=", "1", ";",
                 "Print", "x", ";",
                 "Read", "x", ";",
                 "Call", "secondProcedure", ";",
                 "}"
                });

        REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                           end(expected),
                           [](const std::string l, const std::string o) {
                               return l.compare(o) == 0;
                           }));
    }

    SECTION ("test source program with single condition if loop") {
        std::string source = "procedure testProcedure {"
                             "x = 0;"
                             "if (x <= 1) {"
                             "x = 1;"
                             "} else {"
                             "x = 5;"
                             "}"
                             "}";

        Tokenizer tokenizer = Tokenizer(source);
        auto tokens = tokenizer.tokenize();

//        for(int i = 0; i < tokens.size(); i++)
//            std::cout << tokens[i] << ' ';

        auto expected = std::vector<std::string>(
                {"procedure", "testProcedure", "{", "x", "=", "0", ";", "if", "(", "x", "<=", "1", ")", "{", "x", "=", "1", ";", "}", "else", "{", "x", "=", "5", ";", "}", "}"});

        REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                           end(expected),
                           [](const std::string l, const std::string o) {
                               return l.compare(o) == 0;
                           }));
    }

    SECTION ("test source program with multiple condition if loop") {
        std::string source = "procedure testProcedure {"
                             "x = 0;"
                             "if (x <= 1 || x > 3) {"
                             "x = 1;"
                             "} else {"
                             "x = 5;"
                             "}"
                             "}";

        Tokenizer tokenizer = Tokenizer(source);
        auto tokens = tokenizer.tokenize();

        for(int i = 0; i < tokens.size(); i++)
            std::cout << tokens[i] << ' ';

        auto expected = std::vector<std::string>(
                {"procedure", "testProcedure", "{", "x", "=", "0", ";", "if", "(", "x", "<=", "1", "||", "x", ">", "3", ")", "{", "x", "=", "1", ";", "}", "else", "{", "x", "=", "5", ";", "}", "}"});

        REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                           end(expected),
                           [](const std::string l, const std::string o) {
                               return l.compare(o) == 0;
                           }));
    }

    SECTION ("test source program with while loop") {
        std::string source = "procedure testProcedure {"
                             "x = 0;"
                             "while (x <= 100) {"
                             "x = x + 1;"
                             "}"
                             "}";

        Tokenizer tokenizer = Tokenizer(source);
        auto tokens = tokenizer.tokenize();

//        for(int i = 0; i < tokens.size(); i++)
//            std::cout << tokens[i] << ' ';

        auto expected = std::vector<std::string>(
                {"procedure", "testProcedure", "{", "x", "=", "0", ";", "while", "(", "x", "<=", "100", ")", "{", "x", "=", "x", "+", "1", ";", "}", "}"});

        REQUIRE(std::equal(begin(tokens), end(tokens), begin(expected),
                           end(expected),
                           [](const std::string l, const std::string o) {
                               return l.compare(o) == 0;
                           }));
    }

    SECTION ("test source program with multiple levels of nesting") {
        std::string source = "procedure testProcedure {"
                             "if (x <= 100) {"
                             "while (x != 100) {"
                             "x = x + 1;"
                             "}"
                             "} else {"
                             "Print x;"
                             "}"
                             "}";

        Tokenizer tokenizer = Tokenizer(source);
        auto tokens = tokenizer.tokenize();

//        for(int i = 0; i < tokens.size(); i++)
//            std::cout << tokens[i] << ' ';

        auto expected = std::vector<std::string>(
                {"procedure", "testProcedure", "{", "if", "(", "x", "<=", "100", ")", "{",
                 "while", "(", "x", "!=", "100", ")", "{", "x", "=", "x", "+", "1", ";", "}", "}", "else", "{", "Print", "x", ";", "}", "}"});

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

    SECTION ("Invalid token, throw SPTokenizeException") {
        std::string source = "procedure firstProcedure {"
                             "@ = 1;"
                             "}";

        Tokenizer tokenizer = Tokenizer(source);
        REQUIRE_THROWS_AS(tokenizer.tokenize(), SPTokenizeException);
    }

    SECTION ("Invalid condition in if loop, throw SPTokenizeException") {
        std::string source = "procedure testProcedure {"
                             "x = 0;"
                             "if (x <= 1 & x > 3) {"
                             "x = 1;"
                             "} else {"
                             "x = 5;"
                             "}"
                             "}";

        Tokenizer tokenizer = Tokenizer(source);

//        for(int i = 0; i < tokens.size(); i++)
//            std::cout << tokens[i] << ' ';

        REQUIRE_THROWS_AS(tokenizer.tokenize(), SPTokenizeException);
    }

    SECTION ("Invalid condition in while loop, throw SPTokenizeException") {
        std::string source = "procedure testProcedure {"
                             "x = 0;"
                             "while (x <= 1 | x > 3) {"
                             "x = 1;"
                             "} else {"
                             "x = 5;"
                             "}"
                             "}";

        Tokenizer tokenizer = Tokenizer(source);

//        for(int i = 0; i < tokens.size(); i++)
//            std::cout << tokens[i] << ' ';

        REQUIRE_THROWS_AS(tokenizer.tokenize(), SPTokenizeException);
    }
}