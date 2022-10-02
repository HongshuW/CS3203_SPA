//
// Created by Xingchen Lin on 30/9/22.
//

#pragma once
#ifndef SPA_AFFECTSTCLAUSE_H
#define SPA_AFFECTSTCLAUSE_H

#include "SuchThatClause.h"
#include "query_evaluator/IVisitor.h"
#include "Validatable.h"

using namespace QB;

namespace QB {

    class AffectsTClause : public SuchThatClause, public enable_shared_from_this<AffectsTClause>, public Validatable {
    public:
        AffectsTClause(Ref arg1, Ref arg2);

        bool operator==(const AffectsTClause &other) const;
        Table accept(shared_ptr<IVisitor> visitor) override;
        pair<unordered_set<DesignEntity>, unordered_set<DesignEntity>> getAllowedArgsSynonym() override;
        pair<RefTypeSet, RefTypeSet> getAllowedArgsRefType() override;
    };

} // QB

#endif //SPA_AFFECTSTCLAUSE_H