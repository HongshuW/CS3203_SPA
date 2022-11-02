//
// Created by Nafour on 27/9/22.
//

#ifndef SPA_QUERYTOKENIZERCONSTANTS_H
#define SPA_QUERYTOKENIZERCONSTANTS_H

#pragma once

#include <string>

using namespace std;
namespace QB {

class QueryTokenizerConstants {
 public:
  static const string STMT;
  static const string EMPTY_STR;
  static const string DOUBLE_QUOTE;
  static const string FOLLOWS;
  static const string PARENT;
  static const string CALLS;
  static const string NEXT;
  static const string AFFECTS;
  static const string LEFT_BRACKET;
  static const string RIGHT_BRACKET;
  static const string COMMA;
  static const string UNDERSCORE;
  static const string SEMICOLON;
  static const string LESS_THAN;
  static const string GREATER_THAN;
  static const string DOT;
  static const string EQUAL;

  static const int ZERO;

  static const char HASH;
  static const char STAR;
  static const char DOUBLE_QUOTE_CHAR;
	static const char RIGHT_BRACKET_CHAR;

  static const string PQL_TOKENIZE_EXCEPTION_UNEXPECTED_TOKEN;
  static const string QPL_EXPR_STRING_INVALID_PARENTHESES;
};

}  // namespace QB

#endif  // SPA_QUERYTOKENIZERCONSTANTS_H
