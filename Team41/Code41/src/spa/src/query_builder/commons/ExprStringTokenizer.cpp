//
// Created by Xingchen Lin on 13/9/22.
//

#include "ExprStringTokenizer.h"

using namespace QB;

ExprStringTokenizer::ExprStringTokenizer(string exprStr) : currIdx(0), exprStr(exprStr) {};

char ExprStringTokenizer::peek() {
    char c;
    if (currIdx >= exprStr.length()) {
        c = EOF;
    } else {
        c = exprStr[currIdx];
    }
    return c;
}

char ExprStringTokenizer::pop() {
    char c = peek();
    currIdx++;
    return c;
}

void ExprStringTokenizer::processAlnum() {
    while (isalnum(peek())) {
        curr += pop();
    }
    exprTokens.push_back(curr);
}
vector<string> ExprStringTokenizer::tokenize() {
    char next;

    while (currIdx < exprStr.length()) {
        curr = "";
        next = pop();
        if (next == EOF) break;
        curr += next;

        if (isspace(next)) {
            continue;
        } else if (isalnum(next)) {
            processAlnum();
        } else if (Utils::isBracket(curr)) {
            exprTokens.push_back(curr);
        } else if (Utils::isMathOperators(curr)) {
            exprTokens.push_back(curr);
        } else {
            throw PQLParseException("Unexpected token found in expression: " + next);
        }
    }

    //! Validate the string in expression
    for (int i = 0; i < exprTokens.size(); i++) {
        if (Utils::isValidName(exprTokens[i])) continue;
        if (Utils::isValidNumber(exprTokens[i])) continue;
        if (Utils::isMathOperators(exprTokens[i])) continue;
        if (Utils::isBracket(exprTokens[i])) continue;
        throw PQLParseException("Unexpected token found in expression: " + exprTokens[i]);
    }

    if (!Utils::isValidParentheses(exprTokens)) {
        throw PQLParseException("Invalid parentheses found in expression");
    }

    return exprTokens;
}