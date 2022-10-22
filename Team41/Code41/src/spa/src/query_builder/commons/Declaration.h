//
// Created by Xingchen Lin on 28/8/22.
//

#ifndef SPA_DECLARATION_H
#define SPA_DECLARATION_H

#pragma once

#include <iostream>
#include <ostream>
#include <regex>
#include <string>

#include "query_builder/commons/DesignEntity.h"
#include "query_builder/commons/Synonym.h"

namespace QB {
//! Represents a single declaration in a PQL query (e.g. 'assign a')
class Declaration {
 public:
  DesignEntity designEntity;
  Synonym synonym;
  DesignEntity getDesignEntity() { return designEntity; }
  Synonym getSynonym() { return synonym; }

  Declaration(DesignEntity designEntity, Synonym synonym);

  bool operator==(const Declaration& a2) const;
  static optional<Declaration> findDeclaration(
      Synonym& synonymObj, const shared_ptr<vector<Declaration>>& declarations);
};
}  // namespace QB
#endif  // SPA_DECLARATION_H
