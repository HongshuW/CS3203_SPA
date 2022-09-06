//
// Created by Tan Wei En on 2/9/22.
//

#include <string>
#include <iostream>
#include "Tokenizer.h"

using namespace SourceParser;
using namespace std;

Tokenizer::Tokenizer(std::string source) : currIdx(0), source(source){};

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

std::vector<std::string> Tokenizer::tokenize() {
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
            tokens.push_back(curr);
        } else {
            // throw error here, todo after sprint 0
            cout << curr << endl;
        }
    }
    return tokens;
}
