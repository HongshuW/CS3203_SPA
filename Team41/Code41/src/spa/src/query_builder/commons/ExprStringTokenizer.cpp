//
// Created by Xingchen Lin on 13/9/22.
//

#include "ExprStringTokenizer.h"

#include <utility>

using namespace QB;

ExprStringTokenizer::ExprStringTokenizer(string exprStr) : currIdx(0), exprStr(std::move(exprStr)){}

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

void ExprStringTokenizer::processAlNum() {
    while (isalnum(peek())) {
        curr += pop();
    }
    exprTokens.push_back(curr);
}

bool ExprStringTokenizer::isNotFirstAndLastChar(int index) {
    return index > 0 && index < exprTokens.size() - 1;
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
            processAlNum();
        } else if (Utils::isBracket(curr) || Utils::isMathOperators(curr)) {
            exprTokens.push_back(curr);
        } else {
            throw PQLParseException(QueryTokeniserConstants::PQL_TOKENISE_EXCEPTION_UNEXPECTED_TOKEN + next);
        }
    }

    //! Validate the string in expression
    for (int i = 0; i < exprTokens.size(); i++) {
        if (Utils::isValidName(exprTokens[i])) continue;
        if (Utils::isValidNumber(exprTokens[i])) continue;
        //! To handle this case "+temp", syntax error should be thrown
        //! The first and the last string in the exprTokens cannot be a math operator
        if (Utils::isMathOperators(exprTokens[i]) && isNotFirstAndLastChar(i)) continue;
        if (Utils::isBracket(exprTokens[i])) continue;
        throw PQLParseException(QueryTokeniserConstants::PQL_TOKENISE_EXCEPTION_UNEXPECTED_TOKEN + exprTokens[i]);
    }

    if (!Utils::isValidParentheses(exprTokens)) {
        throw PQLParseException(QueryTokeniserConstants::QPL_EXPR_STRING_INVALID_PARENTHESES);
    }

    return exprTokens;
}