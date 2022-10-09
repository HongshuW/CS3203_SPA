//
// Created by Xingchen Lin on 30/9/22.
//

#include "ModifiesSClause.h"

ModifiesSClause::ModifiesSClause(Ref arg1, Ref arg2) : SuchThatClause(std::move(arg1), std::move(arg2)) {}

bool ModifiesSClause::operator==(const ModifiesSClause &other) const {
    auto clause = dynamic_cast<const ModifiesSClause*>(&other);
    return clause != nullptr && arg1 == clause->arg1 && arg2 == clause->arg2;
}

Table ModifiesSClause::accept(shared_ptr<IVisitor> visitor) {
    return visitor->visit(shared_from_this());
}

pair<unordered_set<DesignEntity>, unordered_set<DesignEntity>> ModifiesSClause::getAllowedArgsSynonym() {
    return make_pair(STMT_SYNONYMS, VARIABLE_SYNONYM);
}

pair<RefTypeSet, RefTypeSet> ModifiesSClause::getAllowedArgsRefType() {
    return make_pair(stmtRefIndexSet, entRefIndexSet);
}