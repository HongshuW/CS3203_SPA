//
// Created by Xingchen Lin on 30/9/22.
//

#ifndef SPA_USESS_H
#define SPA_USESS_H

#include "SuchThatRelations.h"
#include "query_builder/clauses/such_that_clauses/UsesSClause.h"

using namespace QB;

namespace QB {
class UsesS : public SuchThatRelations {
 public:
  shared_ptr<AbstractClause> createClause(Ref arg1, Ref arg2) override;
};
}  // namespace QB

#endif  // SPA_USESS_H
