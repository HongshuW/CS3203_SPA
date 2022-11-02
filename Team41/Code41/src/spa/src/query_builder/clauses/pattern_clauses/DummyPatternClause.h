//
// Created by Xingchen Lin on 2/11/22.
//

#ifndef SPA_DUMMYPATTERNCLAUSE_H
#define SPA_DUMMYPATTERNCLAUSE_H

#pragma once

#include "query_builder/clauses/AbstractClause.h"
#include "query_builder/clauses/pattern_clauses/PatternClause.h"
#include "query_builder/commons/DesignEntity.h"
#include "query_builder/commons/ExpressionSpec.h"
#include "query_builder/commons/Ref.h"
#include "query_builder/constants/QueryParserConstants.h"

using namespace QB;

namespace QB {
class DummyPatternClause : public PatternClause,
                           public enable_shared_from_this<DummyPatternClause> {
 public:
  ExpressionSpec arg3;
  bool isArg3Underscore;
  bool isArg4Underscore;

  DummyPatternClause(Synonym arg1, Ref arg2, ExpressionSpec arg3);
  DummyPatternClause(Synonym arg1, Ref arg2);

  bool operator==(const DummyPatternClause& other) const;
  shared_ptr<Table> accept(shared_ptr<IVisitor> visitor) override;
};
}  // namespace QB

#endif  // SPA_DUMMYPATTERNCLAUSE_H
