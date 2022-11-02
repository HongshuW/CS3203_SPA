//
// Created by Xingchen Lin on 28/9/22.
//

#include "AssignPatternClause.h"

#include <utility>

#include "query_evaluator/IVisitor.h"

AssignPatternClause::AssignPatternClause(Synonym arg1, Ref arg2,
                                         ExpressionSpec arg3)
    : PatternClause(std::move(arg1), std::move(arg2)), arg3(std::move(arg3)) {}

AssignPatternClause::AssignPatternClause(Synonym arg1, Ref arg2)
    : PatternClause(std::move(arg1), std::move(arg2)), arg3(ExpressionSpec()) {}

bool AssignPatternClause::operator==(const AssignPatternClause& other) const {
  auto clause = dynamic_cast<const AssignPatternClause*>(&other);
  return clause != nullptr && arg1 == clause->arg1 && arg2 == clause->arg2 &&
         arg3 == clause->arg3;
}

shared_ptr<Table> AssignPatternClause::accept(shared_ptr<IVisitor> visitor) {
  return visitor->visit(shared_from_this());
}

int AssignPatternClause::validateSyntaxError(int currIdx,
                                             const vector<string>& tokens) {
  return currIdx;
}
