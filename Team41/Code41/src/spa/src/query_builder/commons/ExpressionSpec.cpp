//
// Created by Xingchen Lin on 11/9/22.
//

#include "ExpressionSpec.h"

using namespace QB;

ExpressionSpec::ExpressionSpec(ExpressionSpecType expressionSpecType, shared_ptr<ExprNode> exprNode) :
        expressionSpecType(expressionSpecType),
        exprNode(exprNode) {}

ExpressionSpec::ExpressionSpec(ExpressionSpecType expressionSpecType) :
        expressionSpecType(expressionSpecType) {}

bool ExpressionSpec::operator==(const ExpressionSpec& expressionSpec) const {
    return expressionSpecType == expressionSpec.expressionSpecType &&
           (exprNode == expressionSpec.exprNode || *exprNode == *expressionSpec.exprNode);
}