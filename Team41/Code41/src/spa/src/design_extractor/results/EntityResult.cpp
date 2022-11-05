//
// Created by Nafour on 27/10/22.
//

#include "EntityResult.h"

#include <utility>

DE::EntityResult::EntityResult(shared_ptr<list<string>> result)
				: result(std::move(result)) {}

shared_ptr<list<string>> DE::EntityResult::getResult() { return result; }
