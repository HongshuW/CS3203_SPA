//
// Created by Nafour on 27/9/22.
//

#ifndef SPA_TOKENIZERCONSTANTS_H
#define SPA_TOKENIZERCONSTANTS_H
#include <string>

using namespace std;
namespace SourceParser {

class TokenizerConstants {
 public:
  static const string EMPTY_STR;
  static const string EQUAL;
  static const string GREATER_THAN;
  static const string SMALLER_THAN;
  static const string EXCLAMATION_MARK;
  static const string SINGLE_AND;
  static const string BAR;
  static const string COMMA;
  static const string CURLY_LEFT_BRACKET;
  static const string CURLY_RIGHT_BRACKET;
  static const string PLUS;
  static const string MINUS;
  static const string STAR;
  static const string FORWARD_SLASH;
  static const string MODULO;
  static const string LEFT_BRACKET;
  static const string RIGHT_BRACKET;
  static const string SEMICOLON;

  static const char EQUAL_CHAR;
  static const char SINGLE_AND_CHAR;
  static const char BAR_CHAR;

  static const string UNEXPECTED_TOKEN_MESSAGE;
  static const string INVALID_NUMBER;

  static const int ZERO;
};

}  // namespace SourceParser

#endif  // SPA_TOKENIZERCONSTANTS_H
