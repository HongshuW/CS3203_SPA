//
// Created by Xingchen Lin on 30/9/22.
//

#include "Uses.h"

#include <utility>

Uses::Uses(shared_ptr<vector<Declaration>> declarations)
    : declarations(std::move(declarations)) {}

shared_ptr<AbstractClause> Uses::createClause(Ref arg1, Ref arg2) {
  if (getRefType(arg1) == RefType::INTEGER) {
    // Since arg1 is an integer, it must be a stmtRef
    return make_shared<UsesSClause>(arg1, arg2);
  } else if (getRefType(arg1) == RefType::IDENT) {
    // Since arg1 is an ident, it must be a entRef
    return make_shared<UsesPClause>(arg1, arg2);
  } else if (auto synonym = get_if<Synonym>(&arg1)) {
    // Since arg1 is a synonym, we need to check if the synonym is declared as
    // procedure
    if (auto declaration =
            Declaration::findDeclaration(*synonym, declarations)) {
      if (declaration->getDesignEntity() == DesignEntity::PROCEDURE) {
        return make_shared<UsesPClause>(arg1, arg2);
      } else {
        return make_shared<UsesSClause>(arg1, arg2);
      }
    } else {
      // Arg1 is not declared, return UsesS as default, semantic error will be
      // thrown in validator
      return make_shared<UsesSClause>(arg1, arg2);
    }
  } else {
    // Arg1 is under core, return UsesS as default, semantic error will be
    // thrown in validator
    return make_shared<UsesSClause>(arg1, arg2);
  }
}