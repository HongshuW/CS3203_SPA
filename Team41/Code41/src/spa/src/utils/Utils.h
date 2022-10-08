//
// Created by Xingchen Lin on 9/9/22.
//

#ifndef SPA_UTILSSS_H
#define SPA_UTILSSS_H

#include <string>
#include <regex>
#include <unordered_set>
#include <stack>
#include "parser/constants/ParserConstants.h"

using namespace std;
using namespace SourceParser;

class Utils {
public:
    const static unordered_set<string> BRACKETS;
    const static unordered_set<string> MATH_OPERATORS;
    const static unordered_set<string> VALID_TOKENS_REL_EXPR;
    const static unordered_set<string> VALID_TOKENS_COND_EXPR;

    const static char CHAR_ZERO;
    const static string STRING_ZERO_TO_NINE;

    static bool isValidName(const string& str);
    static bool isValidNumber(string str);
    static bool isMathOperators(const string& str);
    static bool isBracket(const string& str);
    static bool isValidParentheses(vector<string>& tokens);
};

#endif //SPA_UTILSSS_H
