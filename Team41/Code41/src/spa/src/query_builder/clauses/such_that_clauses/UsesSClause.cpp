//
// Created by Xingchen Lin on 30/9/22.
//

#include "UsesSClause.h"

UsesSClause::UsesSClause(Ref arg1, Ref arg2) : SuchThatClause(arg1, arg2) {}

bool UsesSClause::operator==(const UsesSClause &other) const {
    auto clause = dynamic_cast<const UsesSClause*>(&other);
    return clause != nullptr && arg1 == clause->arg1 && arg2 == clause->arg2;
}

Table UsesSClause::accept(shared_ptr<IVisitor> visitor) {
    return visitor->visit(shared_from_this());
}

pair<unordered_set<DesignEntity>, unordered_set<DesignEntity>> UsesSClause::getAllowedArgsSynonym() {
    return make_pair(STMT_SYNONYMS, VARIABLE_SYNONYM);
}

pair<RefTypeSet, RefTypeSet> UsesSClause::getAllowedArgsRefType() {
    return make_pair(stmtRefIndexSet, entRefIndexSet);
}