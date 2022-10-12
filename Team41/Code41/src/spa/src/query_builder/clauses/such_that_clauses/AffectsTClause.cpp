//
// Created by Xingchen Lin on 30/9/22.
//

#include "AffectsTClause.h"

#include <utility>

AffectsTClause::AffectsTClause(Ref arg1, Ref arg2) : SuchThatClause(std::move(arg1), std::move(arg2)) {}

bool AffectsTClause::operator==(const AffectsTClause &other) const {
    auto clause = dynamic_cast<const AffectsTClause*>(&other);
    return clause != nullptr && arg1 == clause->arg1 && arg2 == clause->arg2;
}

Table AffectsTClause::accept(shared_ptr<IVisitor> visitor) {
    return visitor->visit(shared_from_this());
}

pair<unordered_set<DesignEntity>, unordered_set<DesignEntity>> AffectsTClause::getAllowedArgsSynonym() {
    return make_pair(ASSIGN_SYNONYM, ASSIGN_SYNONYM);
}

pair<RefTypeSet, RefTypeSet> AffectsTClause::getAllowedArgsRefType() {
    return make_pair(stmtRefIndexSet, stmtRefIndexSet);
}