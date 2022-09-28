//
// Created by Xingchen Lin on 28/9/22.
//

#ifndef SPA_CLAUSEEVALUATOR_H
#define SPA_CLAUSEEVALUATOR_H

#include "pkb/table/Table.h"
#include "query_builder/clauses/Clause.h"

namespace QE {
    class Clause;

    class ClauseEvaluator {
    public:
        Table evaluateClause(shared_ptr<Clause> clause);
    };

} // QE

#endif //SPA_CLAUSEEVALUATOR_H
