//
// Created by Xingchen Lin on 1/10/22.
//

#ifndef SPA_PATTERNRELATIONS_H
#define SPA_PATTERNRELATIONS_H

#include "query_builder/clauses/AbstractClause.h"
#include "query_builder/commons/Ref.h"
#include "query_builder/commons/Synonym.h"

using namespace QB;

namespace QB {

class PatternRelations {
 public:
  virtual shared_ptr<AbstractClause> createClause(Synonym arg1, Ref arg2);
};

}  // namespace QB

#endif  // SPA_PATTERNRELATIONS_H
