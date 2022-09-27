//
// Created by Tan Wei En on 2/9/22.
//

#include <string>
#include <iostream>
#include "Tokenizer.h"
#include "SPExceptions.h"
#include "constants/TokeniserConstants.h"
#include "utils/ErrorMessageFormatter.h"
#include "constants/ParserConstants.h"
using namespace SourceParser;
using namespace std;

Tokenizer::Tokenizer(std::string source) : currIdx(0), source(source) {};

char Tokenizer::peek() {
    char c;
    if (currIdx >= source.length()) {
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
    tokens.push_back(curr);
}

void Tokenizer::processSymbols() {
    if (curr == TokeniserConstants::EQUAL) {
        if (match(TokeniserConstants::EQUAL_CHAR)) {
            curr += pop();
        }
    } else if (curr == TokeniserConstants::GREATER_THAN) {
        if (match(TokeniserConstants::EQUAL_CHAR)) {
            curr += pop();
        }
    } else if (curr == TokeniserConstants::SMALLER_THAN) {
        if (peek() == TokeniserConstants::EQUAL_CHAR) {
            curr += pop();
        }
    } else if (curr == TokeniserConstants::EXCLAMATION_MARK) {
        if (match(TokeniserConstants::EQUAL_CHAR)) {
            curr += pop();
        }
    } else if (curr == TokeniserConstants::SINGLE_AND) {
        if (!match(TokeniserConstants::SINGLE_AND_CHAR)) {
            string errorMessage = ErrorMessageFormatter::formatErrorMessage(ParserConstants::AND_OP, TokeniserConstants::SINGLE_AND);
            throw SPTokenizeException(errorMessage);
        } else {
            curr += pop();
        }
    } else if (curr == TokeniserConstants::BAR) {
        if (!match(TokeniserConstants::BAR_CHAR)) {
            string errorMessage = ErrorMessageFormatter::formatErrorMessage(ParserConstants::OR_OP, TokeniserConstants::BAR);
            throw SPTokenizeException(errorMessage);
        } else {
            curr += pop();
        }
    }
    tokens.push_back(curr);
}

vector<string> Tokenizer::tokenize() {
    char next;

    while (currIdx < source.length()) {
        curr = TokeniserConstants::EMPTY_STR;
        next = pop();
        if (next == EOF) break;
        curr += next;

        if (isspace(next)) {
            continue;
        } else if (isalnum(next)) {
            processAlNum();
        } else if (next == TokeniserConstants::DOUBLE_QUOTE_CHAR) {
            processString();
        } else if (SYMBOL_SET.count(curr)) {
            processSymbols();
        } else {
            throw SPTokenizeException(TokeniserConstants::UNEXPECTED_TOKEN_MESSAGE +
                                      std::string(1, next));
        }
    }
    return tokens;
}