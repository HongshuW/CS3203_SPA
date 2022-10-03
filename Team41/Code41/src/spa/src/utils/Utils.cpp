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
const unordered_set<string> Utils::CONNECTORS_SYMBOLS = {"&&", "||"};

bool Utils::isValidName(string str) {
    return regex_match(str, NAME_REGEX);
}

bool Utils::isValidNumber(string str) {
    if (str[0] == '-' || (isdigit(str[0]) && isdigit(str[0]) != 0)) {
        return str.substr(1, str.length()).find_first_not_of("0123456789") == std::string::npos;
    } else {
        return false;
    }
}


bool Utils::isMathOperators(string str) {
    return Utils::MATH_OPERATORS.count(str) > 0;
}

bool Utils::isBracket(string str) {
    return Utils::BRACKETS.count(str) > 0;
}

bool Utils::isValidParentheses(vector<string>& tokens) {
    stack<string> stk;
    for (auto str : tokens) {
        if (str.compare("(") == 0) {
            stk.push(str);
        } else if (str.compare(")") == 0) {
            if (stk.empty()) {
                return false;
            }
            stk.pop();
        }
    }
    return stk.empty();
}

bool Utils::isConnector(string str) {
    return Utils::CONNECTORS_SYMBOLS.count(str) > 0;
}