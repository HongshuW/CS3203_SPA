//
// Created by Xingchen Lin on 4/9/22.
//

#include "Ident.h"
#include "query_builder/exceptions/Exceptions.h"

using namespace QB;

Ident::Ident(const string& identStr) : identStr(identStr) {
    if (!Utils::isValidName(identStr)) {
        throw PQLParseException(CommonConstants::PQL_INVALID_IDENT);
    }
}

bool Ident::operator==(const Ident& ident1) const { return identStr == ident1.identStr; }
