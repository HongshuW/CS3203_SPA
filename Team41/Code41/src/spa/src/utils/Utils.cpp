//
// Created by Xingchen Lin on 9/9/22.
//

#include "Utils.h"
#include <unordered_set>

const regex NAME_REGEX = std::regex("[a-zA-Z](\\d|[a-zA-Z])*");

const unordered_set<string> Utils::BRACKETS = {"(", ")"};
const unordered_set<string> Utils::MATH_OPERATORS = {"+", "-", "*", "/", "%"};
const unordered_set<string> Utils::VALID_TOKENS_REL_EXPR = {">", ">=", "<", "<=", "==", "!="};
const unordered_set<string> Utils::VALID_TOKENS_COND_EXPR = {">", ">=", "<", "<=", "==", "!=", "!",
                                                             "+", "-", "*", "/", "(", ")", "&&", "||", "%"};

const char Utils::CHAR_ZERO = '0';
const string Utils::STRING_ZERO_TO_NINE = "0123456789";

bool Utils::isValidName(const string& str) {
    return regex_match(str, NAME_REGEX);
}

bool Utils::isValidNumber(string str) {
    if (str.empty()) return false;
    if (str[ParserConstants::ZERO] == Utils::CHAR_ZERO && str.length() > ParserConstants::ONE) return false;
    return str.find_first_not_of(Utils::STRING_ZERO_TO_NINE) == string::npos;
}


bool Utils::isMathOperators(const string& str) {
    return Utils::MATH_OPERATORS.count(str) > ParserConstants::ZERO;
}

bool Utils::isBracket(const string& str) {
    return Utils::BRACKETS.count(str) > ParserConstants::ZERO;
}

bool Utils::isValidParentheses(vector<string>& tokens) {
    stack<string> stk;
    for (const auto& str : tokens) {
        if (str == ParserConstants::LEFT_BRACKET) {
            stk.push(str);
        } else if (str == ParserConstants::RIGHT_BRACKET) {
            if (stk.empty()) {
                return false;
            }
            stk.pop();
        }
    }
    return stk.empty();
}
