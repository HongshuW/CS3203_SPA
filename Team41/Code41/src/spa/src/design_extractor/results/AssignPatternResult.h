//
// Created by Nafour on 26/10/22.
//

#ifndef SPA_ASSIGNPATTERNRESULT_H
#define SPA_ASSIGNPATTERNRESULT_H
#include <string>

#include "AST/AssignNode.h"
#include "ExtractorResult.h"

using namespace std;
namespace DE {
typedef shared_ptr<vector<pair<pair<int, string>, std::shared_ptr<AssignNode>>>>
    AssignResult;

class AssignPatternResult : public ExtractorResult {
  AssignResult result;

 public:
  explicit AssignPatternResult(AssignResult result);
  AssignResult getResult();
};

}  // namespace DE

#endif  // SPA_ASSIGNPATTERNRESULT_H
