//
// Created by Xingchen Lin on 20/9/22.
//

#include "WithClauses.h"

#include <utility>
namespace QB {
    unordered_map<int, WithRefType> indexToWithRefType({{0, IDENT},
                                                        {1, INT},
                                                        {2, ATTR_REF}});


}


WithClause::WithClause(WithRef lhs, WithRef rhs) : lhs(std::move(lhs)), rhs(std::move(rhs)){}

bool WithClause::isSameWithRefType() {
    return lhs.index() == rhs.index();
}

WithRefType WithClause::lhsType() {
    return indexToWithRefType.at(lhs.index());
}

WithRefType WithClause::rhsType() {
    return indexToWithRefType.at(rhs.index());
}

WithRefType WithClause::getWithRefType(int idx) {
    return indexToWithRefType.at(idx);
}
