//
// Created by Xingchen Lin on 11/9/22.
//

#ifndef SPA_PATTERNCLAUSE_H
#define SPA_PATTERNCLAUSE_H

#pragma once

#include "query_builder/commons/Ref.h"
#include "query_builder/commons/ExpressionSpec.h"
#include "query_builder/commons/DesignEntity.h"
#include "Clause.h"

using namespace QB;

namespace QE {
    class ClauseEvaluator;
}

namespace QB {
    class PatternClause : public Clause {
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

        bool operator==(const Clause& clause) const override;
        ostream& print(ostream & os) const override;
        Table accept(shared_ptr<QE::ClauseEvaluator> clauseEvaluator) override;
    };
}

#endif //SPA_PATTERNCLAUSE_H
