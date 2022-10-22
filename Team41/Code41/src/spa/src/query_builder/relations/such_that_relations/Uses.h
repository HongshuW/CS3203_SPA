//
// Created by Xingchen Lin on 30/9/22.
//

#ifndef SPA_USES_H
#define SPA_USES_H

#include "SuchThatRelations.h"
#include "query_builder/clauses/such_that_clauses/UsesPClause.h"
#include "query_builder/clauses/such_that_clauses/UsesSClause.h"
#include "query_builder/commons/Declaration.h"

using namespace QB;

namespace QB {
class Uses : public SuchThatRelations {
 private:
  shared_ptr<vector<Declaration>> declarations;

 public:
  Uses(shared_ptr<vector<Declaration>> declarations);
  shared_ptr<AbstractClause> createClause(Ref arg1, Ref arg2) override;
};
}  // namespace QB

#endif  // SPA_USES_H
