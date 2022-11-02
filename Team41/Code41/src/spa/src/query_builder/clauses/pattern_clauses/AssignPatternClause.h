//
// Created by Xingchen Lin on 28/9/22.
//

#ifndef SPA_ASSIGNPATTERNCLAUSE_H
#define SPA_ASSIGNPATTERNCLAUSE_H

#pragma once

#include "query_builder/clauses/AbstractClause.h"
#include "query_builder/clauses/pattern_clauses/PatternClause.h"
#include "query_builder/commons/DesignEntity.h"
#include "query_builder/commons/ExpressionSpec.h"
#include "query_builder/commons/Ref.h"
#include "query_builder/constants/QueryParserConstants.h"

using namespace QB;

namespace QB {
class AssignPatternClause
    : public PatternClause,
      public enable_shared_from_this<AssignPatternClause> {
 public:
  ExpressionSpec arg3;

  AssignPatternClause(Synonym arg1, Ref arg2, ExpressionSpec arg3);
  // Create a partial AssignPatternClause
  AssignPatternClause(Synonym arg1, Ref arg2);

  bool operator==(const AssignPatternClause& other) const;
  shared_ptr<Table> accept(shared_ptr<IVisitor> visitor) override;
};
}  // namespace QB

#endif  // SPA_ASSIGNPATTERNCLAUSE_H
