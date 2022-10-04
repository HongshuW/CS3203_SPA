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

WithClause::WithClause(WithRef lhs, WithRef rhs) : lhs(lhs), rhs(rhs) {}

WithRefType WithClause::lhsType() const {
    return indexToWithRefType.at(lhs.index());
}

WithRefType WithClause::rhsType() const {
    return indexToWithRefType.at(rhs.index());
}

WithRefType WithClause::getWithRefType(int idx) {
    return indexToWithRefType.at(idx);
}

bool WithClause::operator==(const WithClause& clause) const {
    auto withClause = dynamic_cast<const WithClause*>(&clause);
    return withClause != nullptr && lhs == withClause->lhs && rhs == withClause->rhs;
}

// For printing
ostream& WithClause::print(ostream& os) const {
    // Print the derived class specific information.
    os << "With clause";
    return os;
}

Table WithClause::accept(shared_ptr<IVisitor> visitor) {
    return visitor->visit(shared_from_this());
}

unordered_set<string> WithClause::getSynonymNames() {
    unordered_set<string> synonyms = unordered_set<string>();
    if (getWithRefType(lhs.index()) == WithRefType::ATTR_REF) synonyms.insert(get<AttrRef>(lhs).synonym.synonym);
    if (getWithRefType(rhs.index()) == WithRefType::ATTR_REF) synonyms.insert(get<AttrRef>(rhs).synonym.synonym);
    return synonyms;
}