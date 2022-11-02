//
// Created by Xingchen Lin on 30/9/22.
//

#pragma once
#ifndef SPA_PARENTCLAUSE_H
#define SPA_PARENTCLAUSE_H

#include "SuchThatClause.h"
#include "Validatable.h"
#include "query_evaluator/IVisitor.h"

using namespace QB;

namespace QB {

class ParentClause : public SuchThatClause,
                     public enable_shared_from_this<ParentClause> {
 public:
  ParentClause(Ref arg1, Ref arg2);

  bool operator==(const ParentClause &other) const;
  shared_ptr<Table> accept(shared_ptr<IVisitor> visitor) override;
  pair<unordered_set<DesignEntity>, unordered_set<DesignEntity>>
  getAllowedArgsSynonym() override;
  pair<RefTypeSet, RefTypeSet> getAllowedArgsRefType() override;
};

}  // namespace QB

#endif  // SPA_PARENTCLAUSE_H
