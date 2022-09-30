//
// Created by Xingchen Lin on 28/9/22.
//

#ifndef SPA_IFPATTERNCLAUSE_H
#define SPA_IFPATTERNCLAUSE_H

#pragma once

#include "query_builder/commons/Synonym.h"
#include "query_builder/commons/Ref.h"
#include "query_builder/clauses/pattern_clauses/PatternClause.h"
#include "query_builder/clauses/AbstractClause.h"

using namespace QB;


namespace QB {

    class IfPatternClause : public PatternClause, public enable_shared_from_this<IfPatternClause> {
    public:
        IfPatternClause(Synonym arg1, Ref arg2);

        bool operator==(const IfPatternClause &clause) const;
        Clause asClauseVariant() override;
        Table accept(shared_ptr<IVisitor> visitor) override;
    };

} // QB

#endif //SPA_IFPATTERNCLAUSE_H
