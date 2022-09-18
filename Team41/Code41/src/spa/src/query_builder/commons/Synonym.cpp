//
// Created by Xingchen Lin on 28/8/22.
//

#include "Synonym.h"
#include <regex>
#include "query_builder/exceptions/Exceptions.h"

using namespace QB;

Synonym::Synonym(string synonym) : synonym(synonym) {
    if (!Utils::isValidName(synonym)) {
        throw PQLParseException("Invalid synonym, unexpected token: " + synonym);
    }
}
