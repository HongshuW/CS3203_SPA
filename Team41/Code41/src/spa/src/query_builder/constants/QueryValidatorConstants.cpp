//
// Created by Xingchen Lin on 28/9/22.
//

#include "QueryValidatorConstants.h"

namespace QB {
const char QueryValidatorConstants::DOT = '.';
const string
    QueryValidatorConstants::PQL_VALIDATION_DUPLICATE_SYNONYM_DECLARATION =
        "Found duplicated synonym for declaration: ";
const string QueryValidatorConstants::PQL_VALIDATION_SYNONYM_NOT_DECLARED =
    "Synonym is not declared: ";
const string QueryValidatorConstants::PQL_VALIDATION_ATTR_REF_NOT_DECLARED =
    "AttrRef is not declared: ";
const string QueryValidatorConstants::PQL_VALIDATION_FIRST_ARG_UNDERSCORE =
    "First argument cannot be underscore for Uses/Modifies Clause as it leads "
    "to ambiguity";
const string QueryValidatorConstants::PQL_VALIDATION_INVALID_PATTERN_SYNTAX =
    "Invalid pattern syntax";
const string
    QueryValidatorConstants::PQL_VALIDATION_INVALID_DESIGN_ENTITY_PATTERN =
        "Expect pattern clause arg2 to be declared as variable, got ";
const string QueryValidatorConstants::PQL_VALIDATION_INVALID_DESIGN_ENTITY =
    "Invalid design entity: ";
const string QueryValidatorConstants::PQL_VALIDATION_INVALID_ATTR_REF =
    "Invalid AttrRef: ";
const string QueryValidatorConstants::PQL_VALIDATION_DIFFERENT_WITH_REF =
    "Two WithRef are different, no comparison can be made";

}  // namespace QB