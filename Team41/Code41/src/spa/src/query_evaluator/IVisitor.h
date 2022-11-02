//
// Created by Nafour on 30/9/22.
//

#pragma once
#ifndef SPA_IVISITOR_H
#define SPA_IVISITOR_H

#include "pkb/table/Table.h"

namespace QB {
class SelectClause;
class IfPatternClause;
class WhilePatternClause;
class AssignPatternClause;
class InvalidPatternClause;
class WithClause;
class FollowsClause;
class FollowsTClause;
class ParentClause;
class ParentTClause;
class UsesSClause;
class UsesPClause;
class ModifiesSClause;
class ModifiesPClause;
class CallsClause;
class CallsTClause;
class NextClause;
class NextTClause;
class AffectsClause;
class AffectsTClause;
}  // namespace QB

class IVisitor {
 public:
  virtual ~IVisitor() = default;
  virtual shared_ptr<Table> visit(shared_ptr<QB::SelectClause> selectClause) = 0;
  virtual shared_ptr<Table> visit(shared_ptr<QB::IfPatternClause> ifPatternClause) = 0;
  virtual shared_ptr<Table> visit(
      shared_ptr<QB::WhilePatternClause> whilePatternClause) = 0;
  virtual shared_ptr<Table> visit(
      shared_ptr<QB::AssignPatternClause> assignPatternClause) = 0;
  virtual shared_ptr<Table> visit(shared_ptr<QB::WithClause> withClause) = 0;
  virtual shared_ptr<Table> visit(shared_ptr<QB::FollowsClause> followsClause) = 0;
  virtual shared_ptr<Table> visit(shared_ptr<QB::FollowsTClause> followsTClause) = 0;
  virtual shared_ptr<Table> visit(shared_ptr<QB::ParentClause> parentClause) = 0;
  virtual shared_ptr<Table> visit(shared_ptr<QB::ParentTClause> parentTClause) = 0;
  virtual shared_ptr<Table> visit(shared_ptr<QB::UsesSClause> usesSClause) = 0;
  virtual shared_ptr<Table> visit(shared_ptr<QB::UsesPClause> usesPClause) = 0;
  virtual shared_ptr<Table> visit(shared_ptr<QB::ModifiesSClause> modifiesSClause) = 0;
  virtual shared_ptr<Table> visit(shared_ptr<QB::ModifiesPClause> modifiesPClause) = 0;
  virtual shared_ptr<Table> visit(shared_ptr<QB::CallsClause> callsClause) = 0;
  virtual shared_ptr<Table> visit(shared_ptr<QB::CallsTClause> callsTClause) = 0;
  virtual shared_ptr<Table> visit(shared_ptr<QB::NextClause> nextClause) = 0;
  virtual shared_ptr<Table> visit(shared_ptr<QB::NextTClause> nextTClause) = 0;
  virtual shared_ptr<Table> visit(shared_ptr<QB::AffectsClause> affectsClause) = 0;
  virtual shared_ptr<Table> visit(shared_ptr<QB::AffectsTClause> affectsTClause) = 0;
};

#endif  // SPA_IVISITOR_H
