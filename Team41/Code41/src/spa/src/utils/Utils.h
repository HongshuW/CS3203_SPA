//
// Created by Xingchen Lin on 9/9/22.
//

#ifndef SPA_UTILSSS_H
#define SPA_UTILSSS_H

#include <string>
#include <regex>
#include <unordered_set>
#include <stack>

using namespace std;

class Utils {
public:
    const static unordered_set<string> MATH_OPERATORS;
    const static unordered_set<string> allowedTokenForRelExprNode;
    const static unordered_set<string> VALID_TOKENS_COND_EXPR;

    static bool isValidName(string str);
    static bool isValidNumber(string str);
    static unordered_set<string> getVariablesFromExprString(string expr);
    static bool isMathOperators(string str);
    static bool isValidParentheses(vector<string>& tokens);
};

#endif //SPA_UTILSSS_H
