//
// Created by Nafour on 30/9/22.
//

#pragma once
#ifndef SPA_CONCRETECLAUSEVISITOR_H
#define SPA_CONCRETECLAUSEVISITOR_H

#include "AbstractClauseVisitor.h"

class ConcreteClauseVisitor : public AbstractClauseVisitor {
 public:
  explicit ConcreteClauseVisitor(
      shared_ptr<QE::DataPreprocessor> dataPreprocessor);
  /**
   * Visit to selectClause is an evaluate of a clause without any condition.
   * @param selectClause
   * @return
   */
  shared_ptr<Table> visit(shared_ptr<QB::SelectClause> selectClause) override;
    shared_ptr<Table> visit(shared_ptr<QB::IfPatternClause> ifPatternClause) override;
    shared_ptr<Table> visit(shared_ptr<QB::WhilePatternClause> WhilePatternClause) override;
    shared_ptr<Table> visit(shared_ptr<QB::AssignPatternClause> assignPatternClause) override;
  /**
   * Visit to with clause checks if there is any synonym in the clause,
   * it will return an empty table if there is no synonym.
   * @param withClause
   * @return
   */
  shared_ptr<Table> visit(shared_ptr<QB::WithClause> withClause) override;
  shared_ptr<Table> visit(shared_ptr<QB::FollowsClause> followsClause) override;
  shared_ptr<Table> visit(shared_ptr<QB::FollowsTClause> followsTClause) override;
  shared_ptr<Table> visit(shared_ptr<QB::ParentClause> parentClause) override;
  shared_ptr<Table> visit(shared_ptr<QB::ParentTClause> parentTClause) override;
  shared_ptr<Table> visit(shared_ptr<QB::UsesSClause> usesSClause) override;
  shared_ptr<Table> visit(shared_ptr<QB::UsesPClause> usesPClause) override;
  shared_ptr<Table> visit(shared_ptr<QB::ModifiesSClause> modifiesSClause) override;
  shared_ptr<Table> visit(shared_ptr<QB::ModifiesPClause> modifiesPClause) override;
  shared_ptr<Table> visit(shared_ptr<QB::CallsClause> callsClause) override;
  shared_ptr<Table> visit(shared_ptr<QB::CallsTClause> callsTClause) override;
  shared_ptr<Table> visit(shared_ptr<QB::NextClause> nextClause) override;
  shared_ptr<Table> visit(shared_ptr<QB::NextTClause> nextTClause) override;
  shared_ptr<Table> visit(shared_ptr<QB::AffectsClause> affectsClause) override;
  shared_ptr<Table> visit(shared_ptr<QB::AffectsTClause> affectsTClause) override;
};

#endif  // SPA_CONCRETECLAUSEVISITOR_H
