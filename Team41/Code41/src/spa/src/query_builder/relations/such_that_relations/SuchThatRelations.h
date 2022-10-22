//
// Created by Xingchen Lin on 30/9/22.
//

#ifndef SPA_SUCHTHATRELATIONS_H
#define SPA_SUCHTHATRELATIONS_H

#include "query_builder/clauses/AbstractClause.h"
#include "query_builder/commons/Ref.h"

using namespace QB;

namespace QB {

class SuchThatRelations {
 public:
  virtual shared_ptr<AbstractClause> createClause(Ref arg1, Ref arg2);
};

}  // namespace QB

#endif  // SPA_SUCHTHATRELATIONS_H
