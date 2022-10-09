//
// Created by Nafour on 27/9/22.
//

#ifndef SPA_PARSERCONSTANTS_H
#define SPA_PARSERCONSTANTS_H
#include <string>

using namespace std;

namespace SourceParser {

    class ParserConstants {

    public:
        static const int ZERO;
        static const int ONE;
        static const int TWO;

        static const string PROCEDURE;
        static const string CURLY_LEFT_BRACKET;
        static const string CURLY_RIGHT_BRACKET;
        static const string IF;
        static const string THEN;
        static const string ELSE;
        static const string READ;
        static const string SEMICOLON;
        static const string EQUAL;
        static const string PRINT;
        static const string WHILE;
        static const string CALL;

        static const string LEFT_BRACKET;
        static const string RIGHT_BRACKET;
        static const string EXCLAMATION_MARK;
        static const string AND_OP;
        static const string OR_OP;
        static const string PLUS;
        static const string MINUS;
        static const string TIMES;
        static const string DIVIDE;
        static const string MODULO;
        static const string LESS_THAN;
        static const string LESS_THAN_AND_EQUAL_TO;
        static const string GREATER_THAN;
        static const string GREATER_THAN_AND_EQUAL_TO;
        static const string EQUAL_TO;
        static const string NOT_EQUAL_TO;

        static const string SP_PARSE_EXCEPTION_EXPECT_COMPARATOR_OP;
        static const string SP_PARSE_EXCEPTION_INVALID_PROC_NAME;
        static const string SP_PARSE_EXCEPTION_EMPTY_STMT_LIST;
        static const string SP_PARSE_EXCEPTION_EXPECT_COND_EXPRESSION;
        static const string SP_PARSE_EXCEPTION_EXPECT_VARIABLE_NAME;
        static const string SP_PARSE_EXCEPTION_ASSIGN_STMT_RHS_EMPTY;
        static const string SP_PARSE_EXCEPTION_EMPTY_PROCEDURE;
        static const string SP_PARSE_EXCEPTION_INVALID_TOKEN_EXPR;
    };
} // SourceParser

#endif //SPA_PARSERCONSTANTS_H
