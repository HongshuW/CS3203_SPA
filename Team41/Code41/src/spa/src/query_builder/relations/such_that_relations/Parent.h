//
// Created by Xingchen Lin on 30/9/22.
//

#ifndef SPA_PARENT_H
#define SPA_PARENT_H

#include "SuchThatRelations.h"
#include "query_builder/clauses/such_that_clauses/ParentClause.h"

using namespace QB;

namespace QB {
    class Parent : public SuchThatRelations {
    public:
        shared_ptr<AbstractClause> createClause(Ref arg1, Ref arg2) override;
    };
} // QB

#endif //SPA_PARENT_H
