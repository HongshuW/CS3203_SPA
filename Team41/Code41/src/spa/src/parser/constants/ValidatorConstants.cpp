//
// Created by Xingchen Lin on 8/10/22.
//

#include "ValidatorConstants.h"

namespace SourceParser {
const string ValidatorConstants::SP_AST_EXCEPTION_CYCLIC_DEPENDENCY =
    "Cyclic dependency found. ";
const string ValidatorConstants::SP_AST_EXCEPTION_DUPLICATE_PROCEDURE =
    "Duplicate procedure found. Procedure: ";
const string ValidatorConstants::SP_AST_EXCEPTION_PROCEDURE_CALLS_ITSELF =
    "Procedure cannot call itself. Procedure: ";
const string ValidatorConstants::SP_AST_EXCEPTION_PROCEDURE_NOT_FOUND =
    "Called procedure does not exist. Procedure: ";
}  // namespace SourceParser