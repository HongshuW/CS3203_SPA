//
// Created by Xingchen Lin on 28/9/22.
//

#include "WhilePatternClause.h"


WhilePatternClause::WhilePatternClause(const Synonym& arg1, const Ref& arg2) : PatternClause(arg1, arg2) {}

bool WhilePatternClause::operator==(const WhilePatternClause &other) const {
    auto clause = dynamic_cast<const WhilePatternClause*>(&other);
    return clause != nullptr && arg1 == clause->arg1
           && arg2 == clause->arg2;
}

Clause WhilePatternClause::asClauseVariant() {
    return shared_from_this();
}

Table WhilePatternClause::accept(shared_ptr<IVisitor> visitor) {
    return visitor->visit(shared_from_this());
}

unsigned int WhilePatternClause::validateSyntaxError(int currIdx, const vector<string>& tokens) {
    expect(QueryParserConstants::UNDERSCORE, currIdx, tokens);
    currIdx++;
    return currIdx;
}
