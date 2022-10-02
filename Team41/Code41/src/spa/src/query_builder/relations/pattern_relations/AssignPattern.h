//
// Created by Xingchen Lin on 1/10/22.
//

#ifndef SPA_ASSIGNPATTERN_H
#define SPA_ASSIGNPATTERN_H
#include "PatternRelations.h"
#include "query_builder/clauses/pattern_clauses/AssignPatternClause.h"
#include "query_builder/constants/QueryParserConstants.h"

namespace QB {

    class AssignPattern : public PatternRelations {
        shared_ptr<AbstractClause> createClause(Synonym arg1, Ref arg2) override;
    };

} // QB

#endif //SPA_ASSIGNPATTERN_H
