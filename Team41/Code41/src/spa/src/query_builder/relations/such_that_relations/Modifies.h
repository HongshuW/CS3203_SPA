//
// Created by Xingchen Lin on 30/9/22.
//

#ifndef SPA_MODIFIES_H
#define SPA_MODIFIES_H

#include "SuchThatRelations.h"
#include "query_builder/clauses/such_that_clauses/ModifiesPClause.h"
#include "query_builder/clauses/such_that_clauses/ModifiesSClause.h"
#include "query_builder/commons/Declaration.h"

using namespace QB;

namespace QB {
class Modifies : public SuchThatRelations {
 private:
  shared_ptr<vector<Declaration>> declarations;

 public:
  Modifies(shared_ptr<vector<Declaration>> declarations);
  shared_ptr<AbstractClause> createClause(Ref arg1, Ref arg2) override;
};
}  // namespace QB

#endif  // SPA_MODIFIES_H
