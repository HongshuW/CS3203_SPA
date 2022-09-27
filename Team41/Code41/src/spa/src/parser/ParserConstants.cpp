//
// Created by Nafour on 27/9/22.
//

#include "ParserConstants.h"

namespace SourceParser {
     const string ParserConstants:: LEFT_BRACKET = "(";
     const string ParserConstants:: RIGHT_BRACKET = ")";
     const string ParserConstants:: LEFT_DIRECTION = "left";
     const string ParserConstants:: RIGHT_DIRECTION = "right";
     const string ParserConstants:: EXCLAMATION_MARK = "!";
     const string ParserConstants:: AND_OP = "&&";
     const string ParserConstants:: OR_OP = "||";
    const string ParserConstants:: AND_OP_OR_OR_OP = "&& or ||";

    const string ParserConstants:: SP_PARSE_EXCEPTION_ERROR_MESSAGE_EXPECT = "Expect: ";
    const string ParserConstants:: SP_PARSE_EXCEPTION_ERROR_MESSAGE_GOT = ", got: ";
    const string ParserConstants:: SP_PARSE_EXCEPTION_EMPTY_RELATIONAL_EXPRESSION = "The relational expression cannot be empty";
    const string ParserConstants:: SP_PARSE_EXCEPTION_INVALID_CONDITIONAL_EXPRESSION = "Invalid expression for conditional expression";
    const string ParserConstants:: SP_PARSE_EXCEPTION_SYNTAX_ERROR = "Syntax errors detected in conditional expression";
    const string ParserConstants:: SP_PARSE_EXCEPTION_EXPECT_COMPARATOR_OP = "comparator operator in conditional expression";
    string ParserConstants::formatErrorMessage(const string& expect, const string& actual) {
        return SP_PARSE_EXCEPTION_ERROR_MESSAGE_EXPECT
        + expect
        + SP_PARSE_EXCEPTION_ERROR_MESSAGE_GOT
        + actual;

    }


} // SourceParser