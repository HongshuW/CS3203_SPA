//
// Created by Xingchen Lin on 28/9/22.
//

#ifndef SPA_WHILEPATTERNCLAUSE_H
#define SPA_WHILEPATTERNCLAUSE_H

#pragma once

#include "query_builder/commons/Synonym.h"
#include "query_builder/commons/Ref.h"
#include "query_builder/clauses/pattern_clauses/PatternClause.h"
#include "pkb/table/Table.h"
#include "query_evaluator/IVisitor.h"
#include "query_builder/clauses/AbstractClause.h"

using namespace QB;

namespace QB {

    class WhilePatternClause : public PatternClause, public enable_shared_from_this<WhilePatternClause> {
    public:
        Synonym arg1; // must be declared as If
        Ref arg2; // entRef -> synonym, _, ident

        WhilePatternClause(const Synonym& arg1, const Ref& arg2);

        bool operator==(const WhilePatternClause &clause) const;
        Clause asClauseVariant() override;
        Table accept(shared_ptr<IVisitor> visitor) override;
    };

} // QB

#endif //SPA_WHILEPATTERNCLAUSE_H
