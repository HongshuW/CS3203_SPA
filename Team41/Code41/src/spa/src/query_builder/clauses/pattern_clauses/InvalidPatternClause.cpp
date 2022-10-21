//
// Created by Xingchen Lin on 3/10/22.
//

#include "InvalidPatternClause.h"

InvalidPatternClause::InvalidPatternClause(const Synonym& arg1, const Ref& arg2)
    : PatternClause(arg1, arg2) {}

bool InvalidPatternClause::operator==(const InvalidPatternClause& other) const {
  auto clause = dynamic_cast<const InvalidPatternClause*>(&other);
  return clause != nullptr && arg1 == clause->arg1 && arg2 == clause->arg2;
}

Table InvalidPatternClause::accept(shared_ptr<IVisitor> visitor) {
  return Table();
}

int InvalidPatternClause::validateSyntaxError(int currIdx,
                                              const vector<string>& tokens) {
  int savedIdx = currIdx;

  if (tokens[currIdx] == QueryParserConstants::UNDERSCORE) {
    currIdx++;
    if (tokens[currIdx] == QueryParserConstants::RIGHT_BRACKET) {
      //! invalid while pattern
      return currIdx;
    } else if (tokens[currIdx] == QueryParserConstants::COMMA) {
      //! invalid if pattern
      currIdx++;
      expect(QueryParserConstants::UNDERSCORE, currIdx, tokens);
      currIdx++;
      return currIdx;
    } else {
      return validateExprSpec(savedIdx, tokens);
    }
  } else {
    return validateExprSpec(savedIdx, tokens);
  }
}

int InvalidPatternClause::validateExprSpec(int currIdx,
                                           const vector<string>& tokens) {
  vector<string> expr;
  if (tokens[currIdx] == QueryParserConstants::DOUBLE_QUOTE) {
    currIdx++;
    //! Full match
    ExprStringTokenizer tokenizer = ExprStringTokenizer(tokens[currIdx]);
    currIdx++;
    expr = tokenizer.tokenize();
    if (expr.empty()) {
      throw PQLParseException(
          QueryParserConstants::PQL_PARSE_EXCEPTION_EMPTY_ASSIGN_EXPRESSION);
    }
    expect(QueryParserConstants::DOUBLE_QUOTE, currIdx, tokens);
    currIdx++;
    ExprNodeParser parser = ExprNodeParser(expr);
    auto exprNode = parser.parse();
    if (exprNode == nullptr) {
      string errorMessage = ErrorMessageFormatter::formatErrorMessage(
          QueryParserConstants::PQL_PARSE_EXCEPTION_EXPECT_EXPRESSION,
          tokens[currIdx]);
      throw PQLParseException(errorMessage);
    }
    return currIdx;
  } else if (tokens[currIdx] == QueryParserConstants::UNDERSCORE) {
    currIdx++;
    if (tokens[currIdx] == QueryParserConstants::RIGHT_BRACKET) {
      //! Any match
      return currIdx;
    } else {
      //! Partial match
      expect(QueryParserConstants::DOUBLE_QUOTE, currIdx, tokens);
      currIdx++;
      ExprStringTokenizer tokenizer = ExprStringTokenizer(tokens[currIdx]);
      expr = tokenizer.tokenize();
      if (expr.empty()) {
        throw PQLParseException(
            QueryParserConstants::PQL_PARSE_EXCEPTION_EMPTY_ASSIGN_EXPRESSION);
      }
      currIdx++;
      expect(QueryParserConstants::DOUBLE_QUOTE, currIdx, tokens);
      currIdx++;
      expect(QueryParserConstants::UNDERSCORE, currIdx, tokens);
      currIdx++;
      ExprNodeParser parser = ExprNodeParser(expr);
      auto exprNode = parser.parse();
      if (exprNode == nullptr) {
        string errorMessage = ErrorMessageFormatter::formatErrorMessage(
            QueryParserConstants::PQL_PARSE_EXCEPTION_EXPECT_EXPRESSION,
            tokens[currIdx]);
        throw PQLParseException(errorMessage);
      }
      return currIdx;
    }
  } else {
    string errorMessage = ErrorMessageFormatter::formatErrorMessage(
        QueryParserConstants::PQL_PARSE_EXCEPTION_EXPECT_EXPRESSION,
        tokens[currIdx]);
    throw PQLParseException(errorMessage);
  }
}