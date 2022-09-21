//
// Created by Xingchen Lin on 11/9/22.
//

#ifndef SPA_PATTERNCLAUSE_H
#define SPA_PATTERNCLAUSE_H

#pragma once

#include "query_builder/commons/Ref.h"
#include "query_builder/commons/ExpressionSpec.h"
#include "query_builder/commons/DesignEntity.h"

using namespace QB;

namespace QB {
    class PatternClause {
    public:
        //! pattern arg1(arg2, arg3)
        DesignEntity patternType; // must be ASSIGN, IF, WHILE
        Synonym arg1; // must be declared as ASSIGN, IF, WHILE
        Ref arg2; // entRef -> synonym, _, ident
        optional<ExpressionSpec> arg3;

        //! For while and if
        PatternClause(DesignEntity patternType, Synonym arg1, Ref arg2);
        //! For assign
        PatternClause(DesignEntity patternType, Synonym arg1, Ref arg2, ExpressionSpec arg3);

        bool operator==(const PatternClause& patternClause) const {
            return patternType == patternClause.patternType &&
                arg1 == patternClause.arg1 && arg2 == patternClause.arg2 && arg3 == patternClause.arg3;
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
