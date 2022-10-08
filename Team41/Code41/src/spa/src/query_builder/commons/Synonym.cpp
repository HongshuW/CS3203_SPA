//
// Created by Xingchen Lin on 28/8/22.
//

#include "Synonym.h"
#include "query_builder/exceptions/Exceptions.h"

using namespace QB;

Synonym::Synonym(const string& synonym) : synonym(synonym) {
    if (!Utils::isValidName(synonym)) {
        throw PQLParseException(CommonConstants::PQL_INVALID_SYNONYM);
    }
}

bool Synonym::operator==(const Synonym& synonym1) const {
    return synonym == synonym1.synonym;
}
