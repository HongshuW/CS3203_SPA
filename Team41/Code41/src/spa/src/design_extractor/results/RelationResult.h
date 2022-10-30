//
// Created by Nafour on 26/10/22.
//

#ifndef SPA_RELATIONRESULT_H
#define SPA_RELATIONRESULT_H

#include <list>
#include <memory>
#include <string>
#include <vector>

#include "ExtractorResult.h"

using namespace std;

namespace DE {
class RelationResult : public ExtractorResult {
  shared_ptr<list<vector<string>>> result;

 public:
  explicit RelationResult(shared_ptr<list<vector<string>>> result);
  shared_ptr<list<vector<string>>> getResult();
};
}  // namespace DE

#endif  // SPA_RELATIONRESULT_H
