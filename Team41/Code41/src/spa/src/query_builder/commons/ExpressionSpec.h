//
// Created by Xingchen Lin on 11/9/22.
//

#ifndef SPA_EXPRESSIONSPEC_H
#define SPA_EXPRESSIONSPEC_H

#pragma once

#include <ostream>

#include "AST/ExprNode.h"
#include "ExpressionSpecType.h"
#include "regex"
#include "string"

using namespace std;
using namespace QB;

namespace QB {

class ExpressionSpec {
 public:
  ExpressionSpecType expressionSpecType;
  shared_ptr<ExprNode> exprNode;

  ExpressionSpec(ExpressionSpecType expressionSpecType,
                 shared_ptr<ExprNode> exprNode);

  //! Only for ANY_MATCH
  ExpressionSpec(ExpressionSpecType expressionSpecType);

  ExpressionSpec();

  bool operator==(const ExpressionSpec& expressionSpec) const;
};

}  // namespace QB

#endif  // SPA_EXPRESSIONSPEC_H
