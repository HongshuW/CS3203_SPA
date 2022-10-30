//
// Created by Xingchen Lin on 27/10/22.
//

#include "QueryTimeResult.h"

#include <vector>
using namespace std;

namespace DE {
QueryTimeResult::QueryTimeResult(shared_ptr<vector<string>> result)
    : result(result) {}
shared_ptr<vector<string>> DE::QueryTimeResult::getResult() { return result; }
}  // namespace DE