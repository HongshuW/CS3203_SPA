//
// Created by Nafour on 26/10/22.
//

#include "RelationResult.h"

#include <utility>
#include <vector>

using namespace std;

DE::RelationResult::RelationResult(shared_ptr<list<vector<string>>> result)
    : result(std::move(result)) {}

shared_ptr<list<vector<string>>> DE::RelationResult::getResult() {
  return result;
}
