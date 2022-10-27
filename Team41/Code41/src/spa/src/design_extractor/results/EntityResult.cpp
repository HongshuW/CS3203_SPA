//
// Created by Nafour on 27/10/22.
//

#include "EntityResult.h"

DE::EntityResult::EntityResult(shared_ptr<list<string>> result)
    : result(result) {}

shared_ptr<list<string>> DE::EntityResult::getResult() { return result; }
