//
// Created by Xingchen Lin on 20/9/22.
//

#include "WithClauses.h"
#include <utility>
namespace QB {
    unordered_map<int, WithRefType> indexToWithRefType({{0, WithRefType::IDENT},
                                                        {1, WithRefType::INTEGER},
                                                        {2, WithRefType::ATTR_REF}});


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

