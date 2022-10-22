//
// Created by Xingchen Lin on 1/10/22.
//

#ifndef SPA_IFPATTERN_H
#define SPA_IFPATTERN_H

#include "PatternRelations.h"
#include "query_builder/clauses/pattern_clauses/IfPatternClause.h"

namespace QB {

class IfPattern : public PatternRelations {
  shared_ptr<AbstractClause> createClause(Synonym arg1, Ref arg2) override;
};

}  // namespace QB

#endif  // SPA_IFPATTERN_H
