//
// Created by Nafour on 26/10/22.
//

#include "RelationResult.h"

DE::RelationResult::RelationResult(shared_ptr<list<vector<string>>> result)
    : result(result) {}
shared_ptr<list<vector<string>>> DE::RelationResult::getResult() {
  return result;
}
