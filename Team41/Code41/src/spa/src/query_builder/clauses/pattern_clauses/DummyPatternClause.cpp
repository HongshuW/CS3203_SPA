//
// Created by Xingchen Lin on 2/11/22.
//

#include "DummyPatternClause.h"

#include <utility>

#include "query_evaluator/IVisitor.h"

DummyPatternClause::DummyPatternClause(Synonym arg1, Ref arg2,
																				 ExpressionSpec arg3)
				: PatternClause(std::move(arg1), std::move(arg2)), arg3(std::move(arg3)),
				isArg3Underscore(false), isArg4Underscore(false) {}

DummyPatternClause::DummyPatternClause(Synonym arg1, Ref arg2)
				: PatternClause(std::move(arg1), std::move(arg2)), arg3(ExpressionSpec()),
					isArg3Underscore(false), isArg4Underscore(false) {}

bool DummyPatternClause::operator==(const DummyPatternClause& other) const {
	auto clause = dynamic_cast<const DummyPatternClause*>(&other);
	return clause != nullptr && arg1 == clause->arg1 && arg2 == clause->arg2 &&
				 arg3 == clause->arg3;
}

Table DummyPatternClause::accept(shared_ptr<IVisitor> visitor) {
	return Table();
}

void DummyPatternClause::validateSyntaxError() {

}