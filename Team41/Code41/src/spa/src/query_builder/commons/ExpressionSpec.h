//
// Created by Xingchen Lin on 11/9/22.
//

#ifndef SPA_EXPRESSIONSPEC_H
#define SPA_EXPRESSIONSPEC_H

#pragma once

#include "ExpressionSpecType.h"
#include "AST/ExprNode.h"
#include "string"
#include "regex"
#include <ostream>

using namespace std;
using namespace QB;

namespace QB {

    class ExpressionSpec {
    public:
        ExpressionSpecType expressionSpecType;
        shared_ptr<ExprNode> exprNode;

        ExpressionSpec(ExpressionSpecType expressionSpecType, shared_ptr<ExprNode> exprNode);

        //! Only for ANY_MATCH
        ExpressionSpec(ExpressionSpecType expressionSpecType);

        bool operator==(const ExpressionSpec& expressionSpec) const;
        friend std::ostream& operator<<(std::ostream& os, ExpressionSpec const& exprSpec) {
            //TODO: to be implemented later once the print method in the ATSNodes are done
            os << exprSpec.exprNode;
            return os;
        };
    };

} // QB

#endif //SPA_EXPRESSIONSPEC_H
