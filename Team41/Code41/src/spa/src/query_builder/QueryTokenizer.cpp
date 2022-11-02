//
// Created by Xingchen Lin on 27/8/22.
//

#include "query_builder/QueryTokenizer.h"

#include <utility>

#include "query_builder/exceptions/Exceptions.h"

using QB::QueryTokenizer;
using namespace std;

QueryTokenizer::QueryTokenizer(string query)
    : currIdx(QueryTokenizerConstants::ZERO), query(std::move(query)) {}

bool QueryTokenizer::isWithinBound() { return currIdx < query.length(); }

char QueryTokenizer::peek() {
  char c;
  if (!isWithinBound()) {
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
  if (curr == QueryTokenizerConstants::STMT &&
      peek() == QueryTokenizerConstants::HASH) {
    curr += pop();
  }
  if (CLAUSE_SET_WITH_T.count(curr) &&
      peek() == QueryTokenizerConstants::STAR) {
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
  tokens.push_back(QueryTokenizerConstants::DOUBLE_QUOTE);
  curr = QueryTokenizerConstants::EMPTY_STR;
  while (peek() != QueryTokenizerConstants::DOUBLE_QUOTE_CHAR) {
    char c = pop();
    if (isspace(c)) continue;
    if (c == EOF) {
      throw PQLTokenizeException(
          QueryTokenizerConstants::PQL_TOKENIZE_EXCEPTION_UNEXPECTED_TOKEN + c);
    }
    curr += c;
  }
  tokens.push_back(curr);
  tokens.push_back(QueryTokenizerConstants::DOUBLE_QUOTE);
  currIdx++;
}

std::vector<std::string> QueryTokenizer::tokenize() {
  char next;

  while (isWithinBound()) {
    curr = QueryTokenizerConstants::EMPTY_STR;
    next = pop();
    curr += next;

    if (isspace(next)) {
      continue;
    } else if (isalpha(next)) {
      processIdent();
    } else if (isdigit(next)) {
      processDigit();
    } else if (next == QueryTokenizerConstants::DOUBLE_QUOTE_CHAR) {
      processString();
    } else if (SYMBOL_SET.count(curr)) {
      tokens.push_back(curr);
    } else {
      throw PQLTokenizeException(
          QueryTokenizerConstants::PQL_TOKENIZE_EXCEPTION_UNEXPECTED_TOKEN +
          next);
    }
  }

  return tokens;
}