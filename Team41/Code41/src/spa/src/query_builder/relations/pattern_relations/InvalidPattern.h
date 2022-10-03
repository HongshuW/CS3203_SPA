//
// Created by Xingchen Lin on 3/10/22.
//

#ifndef SPA_INVALIDPATTERN_H
#define SPA_INVALIDPATTERN_H

#include "PatternRelations.h"
#include "query_builder/clauses/pattern_clauses/InvalidPatternClause.h"
#include "query_builder/constants/QueryParserConstants.h"

namespace QB {

    class InvalidPattern : public PatternRelations {
        shared_ptr<AbstractClause> createClause(Synonym arg1, Ref arg2) override;
    };

} // QB

#endif //SPA_INVALIDPATTERN_H
