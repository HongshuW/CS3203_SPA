//
// Created by Nafour on 27/9/22.
//

#ifndef SPA_PARSERCONSTANTS_H
#define SPA_PARSERCONSTANTS_H
#include <string>

using namespace std;

namespace SourceParser {

    class ParserConstants {
        static const string SP_PARSE_EXCEPTION_ERROR_MESSAGE_EXPECT;
        static const string SP_PARSE_EXCEPTION_ERROR_MESSAGE_GOT;

    public:
        static const string LEFT_BRACKET;
        static const string RIGHT_BRACKET;
        static const string LEFT_DIRECTION;
        static const string RIGHT_DIRECTION;
        static const string EXCLAMATION_MARK;
        static const string AND_OP;
        static const string OR_OP;
        static const string AND_OP_OR_OR_OP;

        static const string SP_PARSE_EXCEPTION_EMPTY_RELATIONAL_EXPRESSION;
        static const string SP_PARSE_EXCEPTION_INVALID_CONDITIONAL_EXPRESSION;
        static const string SP_PARSE_EXCEPTION_SYNTAX_ERROR;
        static const string SP_PARSE_EXCEPTION_EXPECT_COMPARATOR_OP;



        static string formatErrorMessage(const string& expect, const string& actual);


    };

} // SourceParser

#endif //SPA_PARSERCONSTANTS_H
