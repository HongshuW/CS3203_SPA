//
// Created by Xingchen Lin on 4/9/22.
//

#include "Ident.h"
#include "query_builder/Exceptions.h"

using namespace QB;

const std::regex IDENT_REGEX = std::regex("[a-zA-Z](\\d|[a-zA-Z])*");

Ident::Ident(string identStr) : identStr(identStr) {
    if (!Ident::isValidIdent(identStr)) {
        throw PQLParseException("Invalid ident, unexpected token: " + identStr);
    }
}

bool Ident::isValidIdent(std::string str) {
    return std::regex_match(str, IDENT_REGEX);
}
