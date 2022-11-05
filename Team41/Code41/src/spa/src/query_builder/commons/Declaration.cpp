//
// Created by Xingchen Lin on 31/8/22.
//

#include "Declaration.h"

#include <utility>

using namespace std;
using namespace QB;

Declaration::Declaration(DesignEntity designEntity, Synonym synonym)
    : designEntity(designEntity), synonym(std::move(synonym)) {}

DesignEntity Declaration::getDesignEntity() const { return designEntity; }

Synonym Declaration::getSynonym() const { return synonym; }

optional<Declaration> Declaration::findDeclaration(
    Synonym& synonymObj, const shared_ptr<vector<Declaration>>& declarations) {
  for (auto declaration : *declarations) {
    if (declaration.getSynonym().synonym == synonymObj.synonym) {
      return make_optional(declaration);
    }
  }
  return nullopt;
}

bool Declaration::operator==(const Declaration& a2) const {
  return designEntity == a2.designEntity && synonym == a2.synonym;
}