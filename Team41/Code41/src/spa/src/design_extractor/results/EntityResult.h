//
// Created by Nafour on 27/10/22.
//

#ifndef SPA_ENTITYRESULT_H
#define SPA_ENTITYRESULT_H
#include <list>
#include <memory>
#include <string>

#include "design_extractor/results/ExtractorResult.h"
using namespace std;

namespace DE {

class EntityResult : public ExtractorResult {
  shared_ptr<list<string>> result;

 public:
  explicit EntityResult(shared_ptr<list<string>> result);
  shared_ptr<list<string>> getResult();
};

}  // namespace DE

#endif  // SPA_ENTITYRESULT_H
