//
// Created by Xingchen Lin on 11/9/22.
//

#ifndef SPA_PATTERNCLAUSE_H
#define SPA_PATTERNCLAUSE_H

#pragma once

#include "query_builder/commons/Ref.h"
#include "query_builder/commons/ExpressionSpec.h"

using namespace QB;

namespace QB {
    class PatternClause {
    public:
        //! pattern arg1(arg2, arg3)
        Synonym arg1; // must be ASSIGN
        Ref arg2; // entRef -> synonym, _, ident
        ExpressionSpec arg3;

        PatternClause(Synonym arg1, Ref arg2, ExpressionSpec arg3);

        bool operator==(const PatternClause& patternClause) const {
            return arg1 == patternClause.arg1 && arg2 == patternClause.arg2 && arg3 == patternClause.arg3;
        }

        // For printing
        std::ostream & print(std::ostream & os) const {
            //TODO: to be implemented later
            os << "Pattern Clause with expression: ";
            return os;
        }
    };
}

#endif //SPA_PATTERNCLAUSE_H
