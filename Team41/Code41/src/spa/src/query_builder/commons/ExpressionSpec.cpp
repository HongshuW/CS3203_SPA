//
// Created by Xingchen Lin on 11/9/22.
//

#include "ExpressionSpec.h"

#include <utility>

using namespace QB;

ExpressionSpec::ExpressionSpec(ExpressionSpecType expressionSpecType,
                               shared_ptr<ExprNode> exprNode)
    : expressionSpecType(expressionSpecType), exprNode(std::move(exprNode)) {}

ExpressionSpec::ExpressionSpec(ExpressionSpecType expressionSpecType)
    : expressionSpecType(expressionSpecType) {}

ExpressionSpec::ExpressionSpec() : expressionSpecType(ExpressionSpecType::ANY_MATCH ) {}

bool ExpressionSpec::operator==(const ExpressionSpec& expressionSpec) const {
  return expressionSpecType == expressionSpec.expressionSpecType &&
         (exprNode == expressionSpec.exprNode ||
          *exprNode == *expressionSpec.exprNode);
}