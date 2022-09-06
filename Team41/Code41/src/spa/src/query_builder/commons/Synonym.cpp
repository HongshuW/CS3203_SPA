//
// Created by Xingchen Lin on 28/8/22.
//

#include "Synonym.h"
#include <regex>
#include "query_builder/Exceptions.h"

using namespace QB;

const std::regex SYNONYM_REGEX = std::regex("[a-zA-Z](\\d|[a-zA-Z])*");

bool Synonym::isValidSynonym(std::string str) {
    return std::regex_match(str, SYNONYM_REGEX);
}

Synonym::Synonym(std::string synonym) : synonym(synonym) {
    if (!Synonym::isValidSynonym(synonym)) {
        throw PQLParseException("Invalid synonym, unexpected token: " + synonym);
    }
}
