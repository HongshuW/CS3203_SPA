//
// Created by Xingchen Lin on 30/9/22.
//

#ifndef SPA_PATTERNRELATIONS_H
#define SPA_PATTERNRELATIONS_H

#include "query_builder/commons/Ref.h"
#include "query_builder/clauses/AbstractClause.h"
#include "query_builder/commons/ExpressionSpec.h"

namespace QB {

    class PatternRelations {
    public:
        virtual AbstractClause createClause(Synonym arg1, Ref arg2);
        virtual AbstractClause createClause(Synonym arg1, Ref arg2, ExpressionSpec arg3);
    };

} // QB

#endif //SPA_PATTERNRELATIONS_H
