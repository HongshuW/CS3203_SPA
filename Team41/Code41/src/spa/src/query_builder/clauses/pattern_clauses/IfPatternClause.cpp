//
// Created by Xingchen Lin on 28/9/22.
//

#include "IfPatternClause.h"
#include "query_builder/clauses/pattern_clauses/PatternClause.h"
#include "query_evaluator/IVisitor.h"

IfPatternClause::IfPatternClause(Synonym arg1, Ref arg2) : PatternClause(arg1, arg2) {}

bool IfPatternClause::operator==(const IfPatternClause &other) const {
    auto clause = dynamic_cast<const IfPatternClause*>(&other);
    return clause != nullptr && arg1 == clause->arg1
           && arg2 == clause->arg2;
}

Clause IfPatternClause::asClauseVariant() {
    return shared_from_this();
}

Table IfPatternClause::accept(shared_ptr<IVisitor> visitor) {
    return visitor->visit(shared_from_this());
}

unsigned int IfPatternClause::validateSyntaxError(int currIdx, const vector<string>& tokens) {
    expect(QueryParserConstants::UNDERSCORE, currIdx, tokens);
    currIdx++;
    expect(QueryParserConstants::COMMA, currIdx, tokens);
    currIdx++;
    expect(QueryParserConstants::UNDERSCORE, currIdx, tokens);
    currIdx++;
    return currIdx;
}
