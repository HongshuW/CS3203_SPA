//
// Created by Xingchen Lin on 30/9/22.
//

#include "FollowsClause.h"

FollowsClause::FollowsClause(Ref arg1, Ref arg2) : SuchThatClause(std::move(arg1), std::move(arg2)) {}

bool FollowsClause::operator==(const FollowsClause &other) const {
    auto clause = dynamic_cast<const FollowsClause*>(&other);
    return clause != nullptr && arg1 == clause->arg1 && arg2 == clause->arg2;
}

Table FollowsClause::accept(shared_ptr<IVisitor> visitor) {
    return visitor->visit(shared_from_this());
}

pair<unordered_set<DesignEntity>, unordered_set<DesignEntity>> FollowsClause::getAllowedArgsSynonym() {
    return make_pair(STMT_SYNONYMS, STMT_SYNONYMS);
}

pair<RefTypeSet, RefTypeSet> FollowsClause::getAllowedArgsRefType() {
    return make_pair(stmtRefIndexSet, stmtRefIndexSet);
}