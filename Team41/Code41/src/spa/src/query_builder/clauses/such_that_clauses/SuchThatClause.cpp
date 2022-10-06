//
// Created by Xingchen Lin on 31/8/22.
//

#include "SuchThatClause.h"

#include <utility>


using namespace QB;

SuchThatClause::SuchThatClause(Ref arg1, Ref arg2) : arg1(arg1), arg2(arg2) {}

unordered_set<string> SuchThatClause::getSynonymNames() {
    unordered_set<string> synonyms = unordered_set<string>();
    if (getRefType(arg1) == RefType::SYNONYM) synonyms.insert(get<Synonym>(arg1).synonym);
    if (getRefType(arg2) == RefType::SYNONYM) synonyms.insert(get<Synonym>(arg2).synonym);
    return synonyms;
}

