//
// Created by Xingchen Lin on 30/9/22.
//

#pragma once
#ifndef SPA_CALLSTCLAUSE_H
#define SPA_CALLSTCLAUSE_H

#include "SuchThatClause.h"
#include "query_evaluator/IVisitor.h"
#include "Validatable.h"

using namespace QB;

namespace QB {

    class CallsTClause : public SuchThatClause, public enable_shared_from_this<CallsTClause>, public Validatable {
    public:
        CallsTClause(Ref arg1, Ref arg2);

        bool operator==(const CallsTClause &other) const;
        Table accept(shared_ptr<IVisitor> visitor) override;
        pair<unordered_set<DesignEntity>, unordered_set<DesignEntity>> getAllowedArgsSynonym() override;
        pair<RefTypeSet, RefTypeSet> getAllowedArgsRefType() override;
    };

} // QB

#endif //SPA_CALLSTCLAUSE_H