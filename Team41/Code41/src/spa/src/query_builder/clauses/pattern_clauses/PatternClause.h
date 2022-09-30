//
// Created by Xingchen Lin on 11/9/22.
//

#ifndef SPA_PATTERNCLAUSE_H
#define SPA_PATTERNCLAUSE_H

#pragma once
#include "query_builder/commons/Synonym.h"
#include "query_builder/commons/Ref.h"
#include "../AbstractClause.h"

namespace QB {
    class WithClause;
    class SuchThatClause;
    class AssignPatternClause;
    class IfPatternClause;
    class WhilePatternClause;
    class SelectClause;
}

using Clause = std::variant<
//            shared_ptr<PatternClause>,
        shared_ptr<QB::WithClause>,
        shared_ptr<QB::SuchThatClause>,
        shared_ptr<QB::AssignPatternClause>,
        shared_ptr<QB::IfPatternClause>,
        shared_ptr<QB::WhilePatternClause>,
        shared_ptr<QB::SelectClause>>;

namespace QE {
    class ClauseVisitor;
}

using namespace QB;

namespace QB {
    class PatternClause : public AbstractClause {
    public:
        Synonym arg1; // must be declared as If
        Ref arg2; // entRef -> synonym, _, ident
        PatternClause(Synonym arg1, Ref arg2);
        virtual ~PatternClause() = default;
        virtual Clause asClauseVariant() = 0;
        virtual Table accept(shared_ptr<IVisitor> visitor) override = 0;
    };
}

#endif //SPA_PATTERNCLAUSE_H
