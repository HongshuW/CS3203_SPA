//
// Created by Xingchen Lin on 4/9/22.
//

#include "Ident.h"
#include "query_builder/exceptions/Exceptions.h"

using namespace QB;

Ident::Ident(string identStr) : identStr(identStr) {
    if (!Utils::isValidName(identStr)) {
        throw PQLParseException("Invalid ident, unexpected token: " + identStr);
    }
}

bool Ident::operator==(const Ident& ident1) const { return identStr == ident1.identStr; }
