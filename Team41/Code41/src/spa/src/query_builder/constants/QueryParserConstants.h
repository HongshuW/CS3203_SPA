//
// Created by Nafour on 27/9/22.
//

#ifndef SPA_QUERYPARSERCONSTANTS_H
#define SPA_QUERYPARSERCONSTANTS_H

#pragma once

#include <string>

using namespace std;
namespace QB {

class QueryParserConstants {
 public:
  static const string SEMICOLON;
  static const string COMMA;
  static const string BOOLEAN;
  static const string FULL_STOP;
  static const string SMALLER_THAN;
  static const string GREATER_THAN;
  static const string UNDERSCORE;
  static const string DOUBLE_QUOTE;
  static const string LEFT_BRACKET;
  static const string RIGHT_BRACKET;
  static const string SUCH;
  static const string THAT;
  static const string AND;
  static const string PATTERN;
  static const string WITH;
  static const string EQUAL;
  static const string SELECT;

  static const int ZERO;
  static const int ONE;

  static const string PQL_PARSE_EXCEPTION_EMPTY_ASSIGN_EXPRESSION;
  static const string PQL_PARSE_EXCEPTION_EXPECT_EXPRESSION;
  static const string PQL_PARSE_EXCEPTION_SYNONYM_NOT_DECLARED;
  static const string PQL_PARSE_EXCEPTION_EXPECT_SUCH_THAT_OR_PATTERN;
  static const string PQL_PARSE_EXCEPTION_EXPECT_WITH_REF;
  static const string PQL_PARSE_EXCEPTION_EXPECT_REF;
  static const string PQL_PARSE_EXCEPTION_INVALID_PATTERN_SYNTAX;
  static const string PQL_VALIDATION_INVALID_REF_TYPES;
  static const string PQL_VALIDATION_SECOND_ARG_INTEGER;
};

}  // namespace QB

#endif  // SPA_QUERYPARSERCONSTANTS_H
