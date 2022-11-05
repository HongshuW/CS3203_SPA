//
// Created by Nafour on 26/10/22.
//

#include "AssignPatternResult.h"

#include <utility>

namespace DE {
AssignResult AssignPatternResult::getResult() { return result; }

AssignPatternResult::AssignPatternResult(AssignResult result)
    : result(std::move(result)) {}
}  // namespace DE