//
// Created by Xingchen Lin on 11/9/22.
//

#include "ExpressionSpec.h"

using namespace QB;

ExpressionSpec::ExpressionSpec(ExpressionSpecType expressionSpecType, shared_ptr<ExprNode> exprNode) :
        expressionSpecType(expressionSpecType),
        exprNode(exprNode) {};

ExpressionSpec::ExpressionSpec(ExpressionSpecType expressionSpecType) :
        expressionSpecType(expressionSpecType) {};