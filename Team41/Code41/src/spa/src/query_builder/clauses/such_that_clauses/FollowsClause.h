//
// Created by Xingchen Lin on 30/9/22.
//

#pragma once
#ifndef SPA_FOLLOWSCLAUSE_H
#define SPA_FOLLOWSCLAUSE_H

#include "SuchThatClause.h"
#include "Validatable.h"
#include "query_evaluator/IVisitor.h"

using namespace QB;

namespace QB {

class FollowsClause : public SuchThatClause,
                      public enable_shared_from_this<FollowsClause> {
 public:
  FollowsClause(Ref arg1, Ref arg2);

  bool operator==(const FollowsClause &other) const;
  shared_ptr<Table> accept(shared_ptr<IVisitor> visitor) override;
  pair<unordered_set<DesignEntity>, unordered_set<DesignEntity>>
  getAllowedArgsSynonym() override;
  pair<RefTypeSet, RefTypeSet> getAllowedArgsRefType() override;
};

}  // namespace QB

#endif  // SPA_FOLLOWSCLAUSE_H
