//
// Created by Xingchen Lin on 27/10/22.
//

#ifndef SPA_QUERYTIMERESULT_H
#define SPA_QUERYTIMERESULT_H

#include <list>
#include <memory>
#include <string>

#include "ExtractorResult.h"

using namespace std;

namespace DE {

class QueryTimeResult : public ExtractorResult {
  shared_ptr<vector<string>> result;

 public:
  explicit QueryTimeResult(shared_ptr<vector<string>> result);
  shared_ptr<vector<string>> getResult();
};

}  // namespace DE

#endif  // SPA_QUERYTIMERESULT_H
