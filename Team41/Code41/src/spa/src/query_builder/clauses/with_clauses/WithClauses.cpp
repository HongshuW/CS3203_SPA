//
// Created by Xingchen Lin on 20/9/22.
//

#include "WithClauses.h"

#include <utility>

WithClause::WithClause(WithRef lhs, WithRef rhs)
    : lhs(std::move(lhs)), rhs(std::move(rhs)) {}

WithRefType WithClause::lhsType() const {
  return getWithRefTypeFromIndex(lhs.index());
}

WithRefType WithClause::rhsType() const {
  return getWithRefTypeFromIndex(rhs.index());
}

WithRefType WithClause::getWithRefType(int idx) {
  return getWithRefTypeFromIndex(idx);
}

bool WithClause::operator==(const WithClause& clause) const {
  auto withClause = dynamic_cast<const WithClause*>(&clause);
  return withClause != nullptr && lhs == withClause->lhs &&
         rhs == withClause->rhs;
}

Table WithClause::accept(shared_ptr<IVisitor> visitor) {
  return visitor->visit(shared_from_this());
}

unordered_set<string> WithClause::getSynonymNames() {
  unordered_set<string> synonyms = unordered_set<string>();
  if (getWithRefType(lhs.index()) == WithRefType::ATTR_REF)
    synonyms.insert(get<AttrRef>(lhs).synonym.synonym);
  if (getWithRefType(rhs.index()) == WithRefType::ATTR_REF)
    synonyms.insert(get<AttrRef>(rhs).synonym.synonym);
  return synonyms;
}

int WithClause::getValueRefCount() {
    int count = 0;
    if (lhs.index() == WithClause::WITH_REF_IDENT_IDX || lhs.index() == WithClause::WITH_REF_INT_IDX) count++;
    if (rhs.index() == WithClause::WITH_REF_IDENT_IDX || rhs.index() == WithClause::WITH_REF_INT_IDX) count++;
    return count;
}
