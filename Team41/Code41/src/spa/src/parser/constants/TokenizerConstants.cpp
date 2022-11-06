//
// Created by Nafour on 27/9/22.
//

#include "TokenizerConstants.h"

namespace SourceParser {
const string TokenizerConstants::EMPTY_STR;
const string TokenizerConstants::EQUAL = "=";
const string TokenizerConstants::GREATER_THAN = ">";
const string TokenizerConstants::SMALLER_THAN = "<";
const string TokenizerConstants::EXCLAMATION_MARK = "!";
const string TokenizerConstants::SINGLE_AND = "&";
const string TokenizerConstants::BAR = "|";
const string TokenizerConstants::COMMA = ",";
const string TokenizerConstants::CURLY_LEFT_BRACKET = "{";
const string TokenizerConstants::CURLY_RIGHT_BRACKET = "}";
const string TokenizerConstants::PLUS = "+";
const string TokenizerConstants::MINUS = "-";
const string TokenizerConstants::STAR = "*";
const string TokenizerConstants::FORWARD_SLASH = "/";
const string TokenizerConstants::MODULO = "%";
const string TokenizerConstants::LEFT_BRACKET = "(";
const string TokenizerConstants::RIGHT_BRACKET = ")";
const string TokenizerConstants::SEMICOLON = ";";

const char TokenizerConstants::EQUAL_CHAR = '=';
const char TokenizerConstants::SINGLE_AND_CHAR = '&';
const char TokenizerConstants::BAR_CHAR = '|';

const string TokenizerConstants::UNEXPECTED_TOKEN_MESSAGE =
    "Unexpected token: ";
const string TokenizerConstants::INVALID_NUMBER = "Invalid number for token";

const int TokenizerConstants::ZERO = 0;
}  // namespace SourceParser