//
// Created by Xingchen Lin on 30/9/22.
//

#pragma once
#ifndef SPA_MODIFIESSCLAUSE_H
#define SPA_MODIFIESSCLAUSE_H

#include "SuchThatClause.h"
#include "Validatable.h"
#include "query_evaluator/IVisitor.h"

using namespace QB;

namespace QB {

class ModifiesSClause : public SuchThatClause,
                        public enable_shared_from_this<ModifiesSClause> {
 public:
  ModifiesSClause(Ref arg1, Ref arg2);

  bool operator==(const ModifiesSClause &other) const;
  Table accept(shared_ptr<IVisitor> visitor) override;
  pair<unordered_set<DesignEntity>, unordered_set<DesignEntity>>
  getAllowedArgsSynonym() override;
  pair<RefTypeSet, RefTypeSet> getAllowedArgsRefType() override;
};

}  // namespace QB

#endif  // SPA_MODIFIESSCLAUSE_H
