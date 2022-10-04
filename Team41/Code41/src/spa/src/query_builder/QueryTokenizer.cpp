//
// Created by Xingchen Lin on 27/8/22.
//

#include "query_builder/QueryTokenizer.h"

#include <utility>
#include "query_builder/exceptions/Exceptions.h"

using QB::QueryTokenizer;
using namespace std;

QueryTokenizer::QueryTokenizer(string query) : currIdx(0), query(std::move(query)){}

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
    if (curr == QueryTokeniserConstants::STMT && peek() == QueryTokeniserConstants::HASH) {
        curr += pop();
    }
    if (CLAUSE_SET_WITH_T.count(curr) && peek() == QueryTokeniserConstants::STAR) {
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
    tokens.push_back(QueryTokeniserConstants::DOUBLE_QUOTE);
    curr = QueryTokeniserConstants::EMPTY_STR;
    while (peek() != QueryTokeniserConstants::DOUBLE_QUOTE_CHAR) {
        char c = pop();
        if (isspace(c)) continue;
        curr += c;
    }
    tokens.push_back(curr);
    tokens.push_back(QueryTokeniserConstants::DOUBLE_QUOTE);
    currIdx++;
}

std::vector<std::string> QueryTokenizer::tokenize() {
    char next;

    while (currIdx < query.length()) {
        curr = QueryTokeniserConstants::EMPTY_STR;
        next = pop();
        if (next == EOF) break;
        curr += next;

        if (isspace(next)) {
            continue;
        } else if (isalpha(next)) {
            processIdent();
        } else if (isdigit(next)) {
            processDigit();
        } else if (next == QueryTokeniserConstants::DOUBLE_QUOTE_CHAR) {
            processString();
        } else if (SYMBOL_SET.count(curr)) {
            tokens.push_back(curr);
        } else {
            throw PQLTokenizeException(QueryTokeniserConstants::PQL_TOKENISE_EXCEPTION_UNEXPECTED_TOKEN + next);
        }
    }

    return tokens;
}