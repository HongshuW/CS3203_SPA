//
// Created by Xingchen Lin on 1/10/22.
//

#ifndef SPA_WHILEPATTERN_H
#define SPA_WHILEPATTERN_H

#include "PatternRelations.h"
#include "query_builder/clauses/pattern_clauses/WhilePatternClause.h"
#include "query_builder/constants/QueryParserConstants.h"

namespace QB {

class WhilePattern : public PatternRelations {
  shared_ptr<AbstractClause> createClause(Synonym arg1, Ref arg2) override;
};

}  // namespace QB

#endif  // SPA_WHILEPATTERN_H
