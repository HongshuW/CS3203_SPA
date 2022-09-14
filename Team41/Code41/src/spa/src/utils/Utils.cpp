//
// Created by Xingchen Lin on 9/9/22.
//

#include "Utils.h"
#include <unordered_set>

const std::regex NAME_REGEX = std::regex("[a-zA-Z](\\d|[a-zA-Z])*");

const unordered_set<string> Utils::BRACKETS = {"(", ")"};
const unordered_set<string> Utils::MATH_OPERATORS = {"+", "-", "*", "/", "%"};
const unordered_set<string> Utils::allowedTokenForRelExprNode = {">", ">=", "<", "<=", "==", "!="};
const unordered_set<string> Utils::VALID_TOKENS_COND_EXPR = {">", ">=", "<", "<=", "==", "!=", "!",
                                                             "+", "-", "*", "/", "(", ")", "&&", "||"};
const unordered_set<string> Utils::CONNECTORS_SYMBOLS = {"&&", "||"};

bool Utils::isValidName(string str) {
    return regex_match(str, NAME_REGEX);
}

bool Utils::isValidNumber(string str) {
    return str.find_first_not_of("0123456789") == std::string::npos;
}

unordered_set<string> Utils::getVariablesFromExprString(string expr) {
    unordered_set<string> ans;
    unordered_set<char> ignored = {'*', '/', '%', '+', '-', '(', ')', ' ', '=', '<', '>', '!', '|', '&'};
    string acc = "";
    for (int i = 0; i < expr.length(); i++) {
        if (ignored.find(expr[i]) == ignored.end()) { // char not in ignored
            acc += expr[i];
        } else {
            if (!acc.empty() && !isValidNumber(acc)) {
                ans.insert(acc);
            }
            acc = "";
        }
    }
    if (!acc.empty() && !isValidNumber(acc)) {
        ans.insert(acc);
    }
    return ans;
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

bool Utils::isEvenNumber(int num) {
    return num % 2 == 0;
}

bool Utils::isConnector(string str) {
    return Utils::CONNECTORS_SYMBOLS.count(str) > 0;
}