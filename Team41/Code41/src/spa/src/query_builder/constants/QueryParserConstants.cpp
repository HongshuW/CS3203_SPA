//
// Created by Nafour on 27/9/22.
//

#include "QueryParserConstants.h"

namespace QB {

const string QueryParserConstants::SEMICOLON = ";";
const string QueryParserConstants::COMMA = ",";
const string QueryParserConstants::BOOLEAN = "BOOLEAN";
const string QueryParserConstants::FULL_STOP = ".";
const string QueryParserConstants::SMALLER_THAN = "<";
const string QueryParserConstants::GREATER_THAN = ">";
const string QueryParserConstants::UNDERSCORE = "_";
const string QueryParserConstants::DOUBLE_QUOTE = "\"";
const string QueryParserConstants::LEFT_BRACKET = "(";
const string QueryParserConstants::RIGHT_BRACKET = ")";
const string QueryParserConstants::SUCH = "such";
const string QueryParserConstants::THAT = "that";
const string QueryParserConstants::AND = "and";
const string QueryParserConstants::PATTERN = "pattern";
const string QueryParserConstants::EQUAL = "=";
const string QueryParserConstants::SELECT = "Select";
const string QueryParserConstants::WITH = "with";

const int QueryParserConstants::ZERO = 0;
const int QueryParserConstants::ONE = 1;

const string QueryParserConstants::PQL_PARSE_EXCEPTION_EMPTY_ASSIGN_EXPRESSION =
    "Expression for assign statement cannot be empty";
const string QueryParserConstants::PQL_PARSE_EXCEPTION_EXPECT_EXPRESSION =
    "expression";
const string QueryParserConstants::PQL_PARSE_EXCEPTION_EXPECT_REF = "Ref";
const string QueryParserConstants::PQL_PARSE_EXCEPTION_EXPECT_WITH_REF =
    "WithRef";
const string QueryParserConstants::PQL_PARSE_EXCEPTION_SYNONYM_NOT_DECLARED =
    "Synonym is not declared for Pattern Clause: ";
const string
    QueryParserConstants::PQL_PARSE_EXCEPTION_EXPECT_SUCH_THAT_OR_PATTERN =
        "such that or pattern clause";
const string QueryParserConstants::PQL_PARSE_EXCEPTION_INVALID_PATTERN_SYNTAX =
    "Invalid syntax for pattern clause";
}  // namespace QB