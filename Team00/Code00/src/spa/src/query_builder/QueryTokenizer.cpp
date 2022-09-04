//
// Created by Xingchen Lin on 27/8/22.
//

#include "query_builder/QueryTokenizer.h"
#include "query_builder/Exceptions.h"
#include <iostream>

using QB::QueryTokenizer;
using namespace std;

QueryTokenizer::QueryTokenizer(std::string query) : currIdx(0), query(query){};

char QueryTokenizer::peek() {
    char c;
    if (currIdx >= query.length()) {
        c = EOF;
    } else {
        c = query[currIdx];
    }
    return c;
}

char QueryTokenizer::pop() {
    char c = peek();
    currIdx++;
    return c;
}

void QueryTokenizer::processIdent() {
    while (isalnum(peek())) {
        curr += pop();
    }
    if (CLAUSE_SET_WITH_T.count(curr) && peek() == '*') {
        curr += pop();
    }
    tokens.push_back(curr);
}

void QueryTokenizer::processDigit() {
    while (isdigit(peek())) {
        curr += pop();
    }
    tokens.push_back(curr);
}

void QueryTokenizer::processString() {
    tokens.push_back("\"");
    curr = "";
    while (peek() != '"') {
        curr += pop();
    }
    tokens.push_back(curr);
    tokens.push_back("\"");
    currIdx++;
}

std::vector<std::string> QueryTokenizer::tokenize() {
    char next;

    while (currIdx < query.length()) {
        curr = "";
        next = pop();
        if (next == EOF) break;
        curr += next;

        if (isspace(next)) {
            continue;
        } else if (isalpha(next)) {
            processIdent();
        } else if (isdigit(next)) {
            processDigit();
        } else if (next == '"') {
            processString();
        } else if (SYMBOL_SET.count(curr)) {
            tokens.push_back(curr);
        } else {
            throw PQLTokenizeException("Unexpected token " +
                                       std::string(1, next) + "\n");
        }
    }

    return tokens;
}