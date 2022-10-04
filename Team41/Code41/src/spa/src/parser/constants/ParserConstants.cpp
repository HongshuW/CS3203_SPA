//
// Created by Nafour on 27/9/22.
//

#include "ParserConstants.h"

namespace SourceParser {
     const string ParserConstants:: PROCEDURE = "procedure";
     const string ParserConstants:: CURLY_LEFT_BRACKET = "{";
     const string ParserConstants:: CURLY_RIGHT_BRACKET = "}";
     const string ParserConstants:: IF = "if";
     const string ParserConstants:: THEN = "then";
     const string ParserConstants:: ELSE = "else";
     const string ParserConstants:: READ = "read";
     const string ParserConstants:: SEMICOLON = ";";
     const string ParserConstants:: EQUAL = "=";
     const string ParserConstants:: PRINT = "print";
     const string ParserConstants:: WHILE = "while";
     const string ParserConstants:: CALL = "call";
     const string ParserConstants:: LEFT_BRACKET = "(";
     const string ParserConstants:: RIGHT_BRACKET = ")";
     const string ParserConstants:: LEFT_DIRECTION = "left";
     const string ParserConstants:: RIGHT_DIRECTION = "right";
     const string ParserConstants:: EXCLAMATION_MARK = "!";
     const string ParserConstants:: AND_OP = "&&";
     const string ParserConstants:: OR_OP = "||";
    const string ParserConstants:: AND_OP_OR_OR_OP = "&& or ||";
    const string ParserConstants:: EMPTY_STR = "";

    const string ParserConstants:: SP_PARSE_EXCEPTION_EMPTY_RELATIONAL_EXPRESSION = "The relational expression cannot be empty";
    const string ParserConstants:: SP_PARSE_EXCEPTION_INVALID_CONDITIONAL_EXPRESSION = "Invalid expression for conditional expression";
    const string ParserConstants:: SP_PARSE_EXCEPTION_SYNTAX_ERROR = "Syntax errors detected in conditional expression";
    const string ParserConstants:: SP_PARSE_EXCEPTION_EXPECT_COMPARATOR_OP = "comparator operator in conditional expression";
     const string ParserConstants::  SP_PARSE_EXCEPTION_INVALID_PROC_NAME = "Invalid procedure name: ";
     const string ParserConstants::  SP_PARSE_EXCEPTION_EMPTY_STMT_LIST = "Statement list cannot be empty.";
     const string ParserConstants::  SP_PARSE_EXCEPTION_EXPECT_COND_EXPRESSION = "conditional expression";
     const string ParserConstants::  SP_PARSE_EXCEPTION_EXPECT_VARIABLE_NAME = "variable name";
     const string ParserConstants::  SP_PARSE_EXCEPTION_ASSIGN_STMT_RHS_EMPTY = "The RHS of assign statement cannot be empty";
     const string ParserConstants::  SP_PARSE_EXCEPTION_INVALID_PARENTHESES = "Invalid parentheses detected";
     const string ParserConstants::  SP_PARSE_EXCEPTION_EMPTY_PROCEDURE = "No procedure found.";

     const string ParserConstants:: SP_AST_EXCEPTION_CYCLIC_DEPENDENCY = "Cyclic dependency found. ";
    const string ParserConstants:: SP_AST_EXCEPTION_DUPLICATE_PROCEDURE = "Duplicate procedure found. ";
    const string ParserConstants:: SP_AST_EXCEPTION_PROCEDURE_CALLS_ITSELF = "Procedure cannot call itself. ";
    const string ParserConstants:: SP_AST_EXCEPTION_PROCEDURE_NOT_FOUND = "Called procedure does not exist. ";




} // SourceParser