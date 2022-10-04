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
        static const string LEFT_DIRECTION;
        static const string RIGHT_DIRECTION;
        static const string EXCLAMATION_MARK;
        static const string AND_OP;
        static const string OR_OP;
        static const string AND_OP_OR_OR_OP;

        static const string EMPTY_STR;
        static const string SP_PARSE_EXCEPTION_EMPTY_RELATIONAL_EXPRESSION;
        static const string SP_PARSE_EXCEPTION_INVALID_CONDITIONAL_EXPRESSION;
        static const string SP_PARSE_EXCEPTION_SYNTAX_ERROR;
        static const string SP_PARSE_EXCEPTION_EXPECT_COMPARATOR_OP;

        static const string SP_PARSE_EXCEPTION_INVALID_PROC_NAME;
        static const string SP_PARSE_EXCEPTION_EMPTY_STMT_LIST;
        static const string SP_PARSE_EXCEPTION_EXPECT_COND_EXPRESSION;
        static const string SP_PARSE_EXCEPTION_EXPECT_VARIABLE_NAME;
        static const string SP_PARSE_EXCEPTION_ASSIGN_STMT_RHS_EMPTY;
        static const string SP_PARSE_EXCEPTION_INVALID_PARENTHESES;
        static const string SP_PARSE_EXCEPTION_EMPTY_PROCEDURE;
        static const string SP_PARSE_EXCEPTION_INVALID_TOKEN_EXPR;
    };

} // SourceParser

#endif //SPA_PARSERCONSTANTS_H
