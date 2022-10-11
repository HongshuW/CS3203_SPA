//
// Created by Tan Wei En on 2/9/22.
//

#include <string>
#include <utility>
#include "Tokenizer.h"
#include "SPExceptions.h"
#include "constants/TokenizerConstants.h"
#include "utils/ErrorMessageFormatter.h"
#include "constants/ParserConstants.h"
using namespace SourceParser;
using namespace std;

Tokenizer::Tokenizer(std::string source) : currIdx(TokenizerConstants::ZERO), source(std::move(source)) {}

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

void Tokenizer::processAlNum() {
    while (isalnum(peek())) {
        curr += pop();
    }
    tokens.push_back(curr);
}

void Tokenizer::processDigit() {
    while (isdigit(peek())) {
        curr += pop();
    }
    if (!Utils::isValidNumber(curr)) {
        throw SPTokenizeException(TokenizerConstants::INVALID_NUMBER);
    }
    tokens.push_back(curr);
}

void Tokenizer::processSymbols() {
    const unordered_set<string> ALLOWED_TOKENS_FOR_EQUALITY = {
            TokenizerConstants::EQUAL,
            TokenizerConstants::GREATER_THAN,
            TokenizerConstants::SMALLER_THAN,
            TokenizerConstants::EXCLAMATION_MARK

    };
    if (ALLOWED_TOKENS_FOR_EQUALITY.count(curr)) {
        if (match(TokenizerConstants::EQUAL_CHAR)) {
            curr += pop();
        }
    } else if (curr == TokenizerConstants::SINGLE_AND) {
        if (!match(TokenizerConstants::SINGLE_AND_CHAR)) {
            string errorMessage = ErrorMessageFormatter::formatErrorMessage(ParserConstants::AND_OP,
                                                                            TokenizerConstants::SINGLE_AND);
            throw SPTokenizeException(errorMessage);
        }
        curr += pop();
    } else if (curr == TokenizerConstants::BAR) {
        if (!match(TokenizerConstants::BAR_CHAR)) {
            string errorMessage = ErrorMessageFormatter::formatErrorMessage(ParserConstants::OR_OP,
                                                                            TokenizerConstants::BAR);
            throw SPTokenizeException(errorMessage);
        }
        curr += pop();
    }
    tokens.push_back(curr);
}

vector<string> Tokenizer::tokenize() {
    char next;

    while (isWithinBound()) {
        curr = TokenizerConstants::EMPTY_STR;
        next = pop();
        curr += next;
        if (isspace(next)) {
            continue;
        } else if (isalpha(next)) {
            processAlNum();
        } else if (isdigit(next)) {
            processDigit();
        } else if (SYMBOL_SET.count(curr)) {
            processSymbols();
        } else {
            throw SPTokenizeException(TokenizerConstants::UNEXPECTED_TOKEN_MESSAGE + next);
        }
    }
    return tokens;
}