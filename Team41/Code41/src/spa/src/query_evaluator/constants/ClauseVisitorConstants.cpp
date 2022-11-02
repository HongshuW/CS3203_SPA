//
// Created by Nafour on 29/9/22.
//

#include "ClauseVisitorConstants.h"

namespace QE {
const string ClauseVisitorConstants::BOOL_RESULT_HEADER = "$bool_result";
const string ClauseVisitorConstants::TRUE = "true";
const string ClauseVisitorConstants::FALSE = "false";

const Table ClauseVisitorConstants::TRUE_TABLE =
    Table({BOOL_RESULT_HEADER}, {{TRUE}});
const Table ClauseVisitorConstants::FALSE_TABLE =
    Table({BOOL_RESULT_HEADER}, {{FALSE}});

}  // namespace QE