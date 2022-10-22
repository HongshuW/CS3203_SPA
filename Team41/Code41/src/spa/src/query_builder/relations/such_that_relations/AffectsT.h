//
// Created by Xingchen Lin on 30/9/22.
//

#ifndef SPA_AFFECTST_H
#define SPA_AFFECTST_H

#include "SuchThatRelations.h"
#include "query_builder/clauses/such_that_clauses/AffectsTClause.h"

using namespace QB;

namespace QB {
class AffectsT : public SuchThatRelations {
 public:
  shared_ptr<AbstractClause> createClause(Ref arg1, Ref arg2) override;
};
}  // namespace QB

#endif  // SPA_AFFECTST_H
