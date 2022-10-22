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
  Table visit(shared_ptr<QB::SelectClause> selectClause) override;
  Table visit(shared_ptr<QB::IfPatternClause> ifPatternClause) override;
  Table visit(shared_ptr<QB::WhilePatternClause> WhilePatternClause) override;
  Table visit(shared_ptr<QB::AssignPatternClause> assignPatternClause) override;
  /**
   * Visit to with clause checks if there is any synonym in the clause,
   * it will return an empty table if there is no synonym.
   * @param withClause
   * @return
   */
  Table visit(shared_ptr<QB::WithClause> withClause) override;
  Table visit(shared_ptr<QB::FollowsClause> followsClause) override;
  Table visit(shared_ptr<QB::FollowsTClause> followsTClause) override;
  Table visit(shared_ptr<QB::ParentClause> parentClause) override;
  Table visit(shared_ptr<QB::ParentTClause> parentTClause) override;
  Table visit(shared_ptr<QB::UsesSClause> usesSClause) override;
  Table visit(shared_ptr<QB::UsesPClause> usesPClause) override;
  Table visit(shared_ptr<QB::ModifiesSClause> modifiesSClause) override;
  Table visit(shared_ptr<QB::ModifiesPClause> modifiesPClause) override;
  Table visit(shared_ptr<QB::CallsClause> callsClause) override;
  Table visit(shared_ptr<QB::CallsTClause> callsTClause) override;
  Table visit(shared_ptr<QB::NextClause> nextClause) override;
  Table visit(shared_ptr<QB::NextTClause> nextTClause) override;
  Table visit(shared_ptr<QB::AffectsClause> affectsClause) override;
  Table visit(shared_ptr<QB::AffectsTClause> affectsTClause) override;
};

#endif  // SPA_CONCRETECLAUSEVISITOR_H
