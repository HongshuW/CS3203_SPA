//
// Created by Nafour on 27/9/22.
//

#include "ParserConstants.h"

namespace SourceParser {
    const int ParserConstants::ZERO = 0;
    const int ParserConstants::ONE = 1;
    const int ParserConstants::TWO = 2;

    const string ParserConstants::PROCEDURE = "procedure";
    const string ParserConstants::CURLY_LEFT_BRACKET = "{";
    const string ParserConstants::CURLY_RIGHT_BRACKET = "}";
    const string ParserConstants::IF = "if";
    const string ParserConstants::THEN = "then";
    const string ParserConstants::ELSE = "else";
    const string ParserConstants::READ = "read";
    const string ParserConstants::SEMICOLON = ";";
    const string ParserConstants::EQUAL = "=";
    const string ParserConstants::PRINT = "print";
    const string ParserConstants::WHILE = "while";
    const string ParserConstants::CALL = "call";
    const string ParserConstants::LEFT_BRACKET = "(";
    const string ParserConstants::RIGHT_BRACKET = ")";
    const string ParserConstants::EXCLAMATION_MARK = "!";
    const string ParserConstants::AND_OP = "&&";
    const string ParserConstants::OR_OP = "||";
    const string ParserConstants::PLUS = "+";
    const string ParserConstants::MINUS = "-";
    const string ParserConstants::TIMES = "*";
    const string ParserConstants::DIVIDE = "/";
    const string ParserConstants::MODULO = "%";
    const string ParserConstants::LESS_THAN = "<";
    const string ParserConstants::LESS_THAN_AND_EQUAL_TO = "<=";
    const string ParserConstants::GREATER_THAN = ">";
    const string ParserConstants::GREATER_THAN_AND_EQUAL_TO = ">=";
    const string ParserConstants::EQUAL_TO = "==";
    const string ParserConstants::NOT_EQUAL_TO = "!=";

    const string ParserConstants::SP_PARSE_EXCEPTION_EXPECT_COMPARATOR_OP = "Expected a comparator operator in conditional expression";
    const string ParserConstants::SP_PARSE_EXCEPTION_INVALID_PROC_NAME = "Invalid procedure name: ";
    const string ParserConstants::SP_PARSE_EXCEPTION_EMPTY_STMT_LIST = "Statement list cannot be empty.";
    const string ParserConstants::SP_PARSE_EXCEPTION_EXPECT_COND_EXPRESSION = "conditional expression";
    const string ParserConstants::SP_PARSE_EXCEPTION_EXPECT_VARIABLE_NAME = "variable name";
    const string ParserConstants::SP_PARSE_EXCEPTION_ASSIGN_STMT_RHS_EMPTY = "The RHS of assign statement cannot be empty";
    const string ParserConstants::SP_PARSE_EXCEPTION_EMPTY_PROCEDURE = "No procedure found.";
    const string ParserConstants::SP_PARSE_EXCEPTION_INVALID_TOKEN_EXPR = "Invalid Token found for expression";
} // SourceParser