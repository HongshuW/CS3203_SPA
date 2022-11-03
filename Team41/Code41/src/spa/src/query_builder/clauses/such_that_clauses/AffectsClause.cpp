//
// Created by Xingchen Lin on 30/9/22.
//

#include "AffectsClause.h"

#include <utility>

AffectsClause::AffectsClause(Ref arg1, Ref arg2)
    : SuchThatClause(std::move(arg1), std::move(arg2)) {}

bool AffectsClause::operator==(const AffectsClause &other) const {
  auto clause = dynamic_cast<const AffectsClause *>(&other);
  return clause != nullptr && arg1 == clause->arg1 && arg2 == clause->arg2;
}

shared_ptr<Table> AffectsClause::accept(shared_ptr<IVisitor> visitor) {
  return visitor->visit(shared_from_this());
}

pair<unordered_set<DesignEntity>, unordered_set<DesignEntity>>
AffectsClause::getAllowedArgsSynonym() {
  return make_pair(STMT_SYNONYMS, STMT_SYNONYMS);
}

pair<RefTypeSet, RefTypeSet> AffectsClause::getAllowedArgsRefType() {
  return make_pair(stmtRefIndexSet, stmtRefIndexSet);
}