//
// Created by Xingchen Lin on 29/8/22.
//

#pragma once
#ifndef SPA_SUCHTHATCLAUSE_H
#define SPA_SUCHTHATCLAUSE_H

#include "query_builder/commons/Ref.h"
#include "query_builder/commons/RelationType.h"
#include "query_builder/clauses/ConditionalClause.h"
#include "Validatable.h"
#include <ostream>

namespace QB {
    class SuchThatClause : public ConditionalClause, public Validatable {
    public:
        Ref arg1;
        Ref arg2;
        SuchThatClause(Ref arg1, Ref arg2);
        ~SuchThatClause() override = default;
        Table accept(shared_ptr<IVisitor> visitor) override = 0;
        unordered_set<string> getSynonymNames() override;
        pair<unordered_set<DesignEntity>, unordered_set<DesignEntity>> getAllowedArgsSynonym() override = 0;
        pair<RefTypeSet, RefTypeSet> getAllowedArgsRefType() override = 0;
    };
}

#endif //SPA_SUCHTHATCLAUSE_H
