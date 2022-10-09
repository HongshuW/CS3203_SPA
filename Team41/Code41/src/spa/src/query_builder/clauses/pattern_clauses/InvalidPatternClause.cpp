//
// Created by Xingchen Lin on 3/10/22.
//

#include "InvalidPatternClause.h"

InvalidPatternClause::InvalidPatternClause(const Synonym& arg1, const Ref& arg2) : PatternClause(arg1, arg2) {}

bool InvalidPatternClause::operator==(const InvalidPatternClause &other) const {
    auto clause = dynamic_cast<const InvalidPatternClause*>(&other);
    return clause != nullptr && arg1 == clause->arg1
           && arg2 == clause->arg2;
}

Table InvalidPatternClause::accept(shared_ptr<IVisitor> visitor) {
    return Table();
}

unsigned int InvalidPatternClause::validateSyntaxError(int currIdx, const vector<string>& tokens) {
    expect(QueryParserConstants::UNDERSCORE, currIdx, tokens);
    currIdx++;
    if (expect(QueryParserConstants::RIGHT_BRACKET, currIdx, tokens)) {
        return currIdx;
    } else if (expect(QueryParserConstants::COMMA, currIdx, tokens)) {
        currIdx++;
        expect(QueryParserConstants::UNDERSCORE, currIdx, tokens);
        currIdx++;
        return currIdx;
    } else {
        throw PQLParseException(QueryParserConstants::PQL_PARSE_EXCEPTION_INVALID_PATTERN_SYNTAX);
    }
}