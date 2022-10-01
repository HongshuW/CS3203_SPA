//
// Created by Xingchen Lin on 30/9/22.
//

#ifndef SPA_NEXT_H
#define SPA_NEXT_H

#include "SuchThatRelations.h"
#include "query_builder/clauses/such_that_clauses/NextClause.h"

using namespace QB;

namespace QB {
    class Next : public SuchThatRelations {
    public:
        shared_ptr<AbstractClause> createClause(Ref arg1, Ref arg2) override;
    };
} // QB

#endif //SPA_NEXT_H
