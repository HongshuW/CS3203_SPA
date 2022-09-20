//
// Created by Xingchen Lin on 20/9/22.
//

#include "WithClauses.h"

#include <utility>

WithClause::WithClause(WithRef lhs, WithRef rhs) : lhs(std::move(lhs)), rhs(std::move(rhs)){}

bool WithClause::isSameWithRefType() {
    return lhs.index() == rhs.index();
}