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
#include "query_builder/constants/QueryParserConstants.h"

using namespace QB;

namespace QB {

    class WhilePatternClause : public PatternClause, public enable_shared_from_this<WhilePatternClause> {
    public:
        WhilePatternClause(const Synonym& arg1, const Ref& arg2);

        bool operator==(const WhilePatternClause &other) const;
        Table accept(shared_ptr<IVisitor> visitor) override;
        int validateSyntaxError(int currIdx, const vector<string>& tokens) override;
    };

} // QB

#endif //SPA_WHILEPATTERNCLAUSE_H
