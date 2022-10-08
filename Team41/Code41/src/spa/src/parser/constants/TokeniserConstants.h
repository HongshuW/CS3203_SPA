//
// Created by Nafour on 27/9/22.
//

#ifndef SPA_TOKENISERCONSTANTS_H
#define SPA_TOKENISERCONSTANTS_H
#include <string>

using namespace std;
namespace SourceParser {

    class TokeniserConstants {
    public:
        static const string EMPTY_STR;
        static const string DOUBLE_QUOTE;
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

        static const char DOUBLE_QUOTE_CHAR;
        static const char EQUAL_CHAR;
        static const char GREATER_THAN_CHAR;
        static const char SMALLER_THAN_CHAR;
        static const char EXCLAMATION_MARK_CHAR;
        static const char SINGLE_AND_CHAR;
        static const char BAR_CHAR;

        static const string UNEXPECTED_TOKEN_MESSAGE;
        static const string INVALID_NAME;
        static const string INVALID_NUMBER;

        static const unsigned int ZERO;
    };

} // SourceParser

#endif //SPA_TOKENISERCONSTANTS_H
