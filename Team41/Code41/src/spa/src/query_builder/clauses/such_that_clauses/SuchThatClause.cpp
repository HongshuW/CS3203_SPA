//
// Created by Xingchen Lin on 31/8/22.
//

#include "SuchThatClause.h"

#include <utility>

using namespace QB;

SuchThatClause::SuchThatClause(Ref arg1, Ref arg2)
    : arg1(std::move(arg1)), arg2(std::move(arg2)) {}

unordered_set<string> SuchThatClause::getSynonymNames() {
  unordered_set<string> synonyms = unordered_set<string>();
  if (getRefType(arg1) == RefType::SYNONYM)
    synonyms.insert(get<Synonym>(arg1).synonym);
  if (getRefType(arg2) == RefType::SYNONYM)
    synonyms.insert(get<Synonym>(arg2).synonym);
  return synonyms;
}

int SuchThatClause::getValueRefCount() {
  int count = 0;
  RefType ref1Type = getRefType(arg1);
  RefType ref2Type = getRefType(arg2);
  if (ref1Type == RefType::INTEGER || ref1Type == RefType::IDENT) count++;
  if (ref2Type == RefType::INTEGER || ref2Type == RefType::IDENT) count++;
  return count;
}
