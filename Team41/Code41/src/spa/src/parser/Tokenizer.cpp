//
// Created by Tan Wei En on 2/9/22.
//

#include <string>
#include <iostream>
#include "Tokenizer.h"
#include "SPExceptions.h"

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
    if (peek() == c) {
        return true;
    }
    return false;
}

void Tokenizer::processString() {
    tokens.push_back("\"");
    curr = "";
    while (peek() != '"') {
        curr += pop();
    }
    tokens.push_back(curr);
    tokens.push_back("\"");
    currIdx++;
}

void Tokenizer::processAlNum() {
    while (isalnum(peek())) {
        curr += pop();
    }
    tokens.push_back(curr);
}

void Tokenizer::processSymbols() {
    // maybe consider the case of && and ||
    if (curr == "=") {
        if (match('=')) {
            curr += pop();
        }
    } else if (curr == ">") {
        if (match('=')) {
            curr += pop();
        }
    } else if (curr == "<") {
        if (peek() == '=') {
            curr += pop();
        }
    } else if (curr == "!") {
        if (match('=')) {
            curr += pop();
        }
    } else if (curr == "&") {
        if (!match('&')) {
            throw SPTokenizeException("Expect && but got &");
        } else {
            curr += pop();
        }
    } else if (curr == "|") {
        if (!match('|')) {
            throw SPTokenizeException("Expect || but got |");
        } else {
            curr += pop();
        }
    }
    tokens.push_back(curr);
}

vector<string> Tokenizer::tokenize() {
    char next;

    while (currIdx < source.length()) {
        curr = "";
        next = pop();
        if (next == EOF) break;
        curr += next;

        if (isspace(next)) {
            continue;
        } else if (isalnum(next)) {
            processAlNum();
        } else if (next == '"') {
            processString();
        } else if (SYMBOL_SET.count(curr)) {
            processSymbols();
        } else {
            throw SPTokenizeException("Unexpected token " +
                                      std::string(1, next) + "\n");
        }
    }
    return tokens;
}