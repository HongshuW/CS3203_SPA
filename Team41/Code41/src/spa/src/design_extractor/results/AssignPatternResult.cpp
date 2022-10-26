//
// Created by Nafour on 26/10/22.
//

#include "AssignPatternResult.h"

namespace DE {
AssignResult AssignPatternResult::getResult() { return result; }

AssignPatternResult::AssignPatternResult(AssignResult result)
    : result(result) {}
}  // namespace DE