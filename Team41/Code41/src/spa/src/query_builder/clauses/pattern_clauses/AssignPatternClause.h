//
// Created by Xingchen Lin on 28/9/22.
//

#ifndef SPA_ASSIGNPATTERNCLAUSE_H
#define SPA_ASSIGNPATTERNCLAUSE_H

#pragma once

#include "query_builder/commons/Ref.h"
#include "query_builder/commons/DesignEntity.h"
#include "query_builder/commons/ExpressionSpec.h"
#include "query_builder/clauses/pattern_clauses/PatternClause.h"

using namespace QB;

namespace QB {
    class AssignPatternClause : public PatternClause, public enable_shared_from_this<AssignPatternClause>{
    public:
        Synonym arg1; // must be declared as ASSIGN
        Ref arg2; // entRef -> synonym, _, ident
        ExpressionSpec arg3;

        AssignPatternClause(Synonym arg1, Ref arg2, ExpressionSpec arg3);

        bool operator==(const AssignPatternClause &clause) const;
        Clause asClauseVariant() override;
    };
}


#endif //SPA_ASSIGNPATTERNCLAUSE_H
