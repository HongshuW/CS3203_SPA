//
// Created by Xingchen Lin on 30/9/22.
//

#ifndef SPA_SUCHTHATRELATIONS_H
#define SPA_SUCHTHATRELATIONS_H

#include "query_builder/commons/Ref.h"
#include "query_builder/clauses/AbstractClause.h"

namespace QB {

    class SuchThatRelations {
    public:
        virtual shared_ptr<AbstractClause> createClause(Ref arg1, Ref arg2);
    };

} // QB

#endif //SPA_SUCHTHATRELATIONS_H
