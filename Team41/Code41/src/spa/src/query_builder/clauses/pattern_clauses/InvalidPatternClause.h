//
// Created by Xingchen Lin on 3/10/22.
//

#ifndef SPA_INVALIDPATTERNCLAUSE_H
#define SPA_INVALIDPATTERNCLAUSE_H

#pragma once

#include "query_builder/commons/Synonym.h"
#include "query_builder/commons/Ref.h"
#include "query_builder/clauses/pattern_clauses/PatternClause.h"
#include "pkb/table/Table.h"
#include "query_evaluator/IVisitor.h"
#include "query_builder/clauses/AbstractClause.h"
#include "query_builder/constants/QueryParserConstants.h"
#include "utils/ErrorMessageFormatter.h"
#include "query_builder/exceptions/Exceptions.h"

using namespace QB;

namespace QB {

    class InvalidPatternClause : public PatternClause, public enable_shared_from_this<InvalidPatternClause> {
    public:
        InvalidPatternClause(const Synonym& arg1, const Ref& arg2);

        bool operator==(const InvalidPatternClause &other) const;
        Table accept(shared_ptr<IVisitor> visitor) override;
        unsigned int validateSyntaxError(int currIdx, const vector<string>& tokens) override;
    };

} // QB

#endif //SPA_INVALIDPATTERNCLAUSE_H
