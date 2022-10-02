//
// Created by Xingchen Lin on 30/9/22.
//

#pragma once
#ifndef SPA_PARENTCLAUSE_H
#define SPA_PARENTCLAUSE_H

#include "SuchThatClause.h"
#include "query_evaluator/IVisitor.h"
#include "Validatable.h"

using namespace QB;

namespace QB {

    class ParentClause : public SuchThatClause, public enable_shared_from_this<ParentClause>, public Validatable {
    public:
        ParentClause(Ref arg1, Ref arg2);

        bool operator==(const ParentClause &other) const;
        Table accept(shared_ptr<IVisitor> visitor) override;
        pair<unordered_set<DesignEntity>, unordered_set<DesignEntity>> getAllowedArgsSynonym() override;
        pair<RefTypeSet, RefTypeSet> getAllowedArgsRefType() override;
    };

} // QB

#endif //SPA_PARENTCLAUSE_H
