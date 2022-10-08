//
// Created by Tan Wei En on 2/9/22.
//

#include <string>
#include <utility>
#include "Tokenizer.h"
#include "SPExceptions.h"
#include "constants/TokeniserConstants.h"
#include "utils/ErrorMessageFormatter.h"
#include "constants/ParserConstants.h"
using namespace SourceParser;
using namespace std;

Tokenizer::Tokenizer(std::string source) : source(std::move(source)) {}

bool Tokenizer::isWithinBound() {
    return currIdx < source.length();
}

char Tokenizer::peek() {
    char c;
    if (!isWithinBound()) {
        c = EOF;
    } else {
        c = source[currIdx];
    }
    return c;
}

char Tokenizer::pop() {
    char c = peek();
    currIdx++;
    return c;
}

bool Tokenizer::match(char c) {
    return peek() == c;
}

void Tokenizer::processString() {
    tokens.push_back(TokeniserConstants::DOUBLE_QUOTE);
    curr = TokeniserConstants::EMPTY_STR;
    while (peek() != TokeniserConstants::DOUBLE_QUOTE_CHAR) {
        curr += pop();
    }
    tokens.push_back(curr);
    tokens.push_back(TokeniserConstants::DOUBLE_QUOTE);
    currIdx++;
}

void Tokenizer::processAlNum() {
    while (isalnum(peek())) {
        curr += pop();
    }
    if (!Utils::isValidName(curr)) {
        throw SPTokenizeException(TokeniserConstants::INVALID_NAME);
    }
    tokens.push_back(curr);
}

void Tokenizer::processDigit() {
    while (isdigit(peek())) {
        curr += pop();
    }
    if (!Utils::isValidNumber(curr)) {
        throw SPTokenizeException(TokeniserConstants::INVALID_NUMBER);
    }
    tokens.push_back(curr);
}

void Tokenizer::processSymbols() {
    const unordered_set<string> ALLOWED_TOKENS_FOR_EQUALITY = {
            TokeniserConstants::EQUAL,
            TokeniserConstants::GREATER_THAN,
            TokeniserConstants::SMALLER_THAN,
            TokeniserConstants::EXCLAMATION_MARK

    };
    if (ALLOWED_TOKENS_FOR_EQUALITY.count(curr)) {
        if (match(TokeniserConstants::EQUAL_CHAR)) {
            curr += pop();
        }
    } else if (curr == TokeniserConstants::SINGLE_AND) {
        if (!match(TokeniserConstants::SINGLE_AND_CHAR)) {
            string errorMessage = ErrorMessageFormatter::formatErrorMessage(ParserConstants::AND_OP,
                                                                            TokeniserConstants::SINGLE_AND);
            throw SPTokenizeException(errorMessage);
        }
        curr += pop();
    } else if (curr == TokeniserConstants::BAR) {
        if (!match(TokeniserConstants::BAR_CHAR)) {
            string errorMessage = ErrorMessageFormatter::formatErrorMessage(ParserConstants::OR_OP,
                                                                            TokeniserConstants::BAR);
            throw SPTokenizeException(errorMessage);
        }
        curr += pop();
    }
    tokens.push_back(curr);
}

vector<string> Tokenizer::tokenize() {
    char next;

    while (isWithinBound()) {
        curr = TokeniserConstants::EMPTY_STR;
        next = pop();
        if (next == EOF) break;
        curr += next;
        if (isspace(next)) {
            continue;
        } else if (isalpha(next)) {
            processAlNum();
        } else if (isdigit(next)) {
            processDigit();
        } else if (next == TokeniserConstants::DOUBLE_QUOTE_CHAR) {
            processString();
        } else if (SYMBOL_SET.count(curr)) {
            processSymbols();
        } else {
            throw SPTokenizeException(TokeniserConstants::UNEXPECTED_TOKEN_MESSAGE + next);
        }
    }
    return tokens;
}