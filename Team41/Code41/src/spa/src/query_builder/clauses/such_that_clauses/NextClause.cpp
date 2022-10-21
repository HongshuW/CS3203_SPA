//
// Created by Xingchen Lin on 30/9/22.
//

#include "NextClause.h"

NextClause::NextClause(Ref arg1, Ref arg2)
    : SuchThatClause(std::move(arg1), std::move(arg2)) {}

bool NextClause::operator==(const NextClause &other) const {
  auto clause = dynamic_cast<const NextClause *>(&other);
  return clause != nullptr && arg1 == clause->arg1 && arg2 == clause->arg2;
}

Table NextClause::accept(shared_ptr<IVisitor> visitor) {
  return visitor->visit(shared_from_this());
}

pair<unordered_set<DesignEntity>, unordered_set<DesignEntity>>
NextClause::getAllowedArgsSynonym() {
  return make_pair(STMT_SYNONYMS, STMT_SYNONYMS);
}

pair<RefTypeSet, RefTypeSet> NextClause::getAllowedArgsRefType() {
  return make_pair(stmtRefIndexSet, stmtRefIndexSet);
}