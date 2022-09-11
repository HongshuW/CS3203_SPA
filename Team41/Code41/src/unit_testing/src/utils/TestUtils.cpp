//
// Created by Xingchen Lin on 11/9/22.
//

#include "catch.hpp"

#include "utils/Utils.h"

using namespace std;

TEST_CASE("Test QEUtils") {
    SECTION("x + 1") {
        vector<string> tokens = vector<string>(
                {"x", "+", "1"});
        bool isValid = Utils::isValidParentheses(tokens);
        REQUIRE(isValid == true);
    }

    SECTION("(x + 1)") {
        vector<string> tokens = vector<string>(
                {"(", "x", "+", "1", ")"});
        bool isValid = Utils::isValidParentheses(tokens);
        REQUIRE(isValid == true);
    }

    SECTION("(x + 1))") {
        vector<string> tokens = vector<string>(
                {"(", "x", "+", "1", ")", ")"});
        bool isValid = Utils::isValidParentheses(tokens);
        REQUIRE(isValid == false);
    }

    SECTION("((x + 1)") {
        vector<string> tokens = vector<string>(
                {"(", "(", "x", "+", "1", ")"});
        bool isValid = Utils::isValidParentheses(tokens);
        REQUIRE(isValid == false);
    }

    SECTION("(x == y) && (z > 2)") {
        vector<string> tokens = vector<string>(
                {"(", "x", "==", "y", ")", "&&", "(", "z", ">", "2", ")"});
        bool isValid = Utils::isValidParentheses(tokens);
        REQUIRE(isValid == true);
    }

    SECTION("!(x + 1 <= y)") {
        vector<string> tokens = vector<string>(
                {"!", "(", "x", "+", "1", "<=", "y", ")"});
        bool isValid = Utils::isValidParentheses(tokens);
        REQUIRE(isValid == true);
    }

    SECTION("!(x + 1 <= (y)") {
        vector<string> tokens = vector<string>(
                {"!", "(", "x", "+", "1", "<=", "(", "y", ")"});
        bool isValid = Utils::isValidParentheses(tokens);
        REQUIRE(isValid == false);
    }

    SECTION("((x == y) || (x <= 3)) && (z > 2)") {
        vector<string> tokens = vector<string>(
                {"(", "(", "x", "==", "y", ")", "||", "(", "x", "<=", "3", ")", ")", "&&", "(", "z", ">", "2", ")"});
        bool isValid = Utils::isValidParentheses(tokens);
        REQUIRE(isValid == true);
    }
}
