//
// Created by Nafour on 30/9/22.
//

#include "ConcreteClauseVisitor.h"

#include <utility>

#include "query_builder/clauses/pattern_clauses/AssignPatternClause.h"
#include "query_builder/clauses/pattern_clauses/IfPatternClause.h"
#include "query_builder/clauses/such_that_clauses/FollowsClause.h"
ConcreteClauseVisitor::ConcreteClauseVisitor(
    shared_ptr<QE::DataPreprocessor> dataPreprocessor)
    : AbstractClauseVisitor(std::move(dataPreprocessor)) {}

// only for no condition select clause
shared_ptr<Table> ConcreteClauseVisitor::visit(
    shared_ptr<QB::SelectClause> selectClause) {
  return dataPreprocessor->getNoConditionSelectClauseResult(selectClause);
}

shared_ptr<Table> ConcreteClauseVisitor::visit(
    shared_ptr<QB::IfPatternClause> ifPatternClause) {
  return dataPreprocessor->getIfPatternTable(ifPatternClause);
}

shared_ptr<Table> ConcreteClauseVisitor::visit(
    shared_ptr<QB::WhilePatternClause> whilePatternClause) {
  return dataPreprocessor->getWhilePatternTable(whilePatternClause);
}

shared_ptr<Table> ConcreteClauseVisitor::visit(
    shared_ptr<QB::WithClause> withClause) {
  return dataPreprocessor->getTableByWith(withClause);
}

shared_ptr<Table> ConcreteClauseVisitor::visit(
    shared_ptr<QB::FollowsClause> followsClause) {
  return dataPreprocessor->getTableByFollows(followsClause);
}

shared_ptr<Table> ConcreteClauseVisitor::visit(
    shared_ptr<QB::FollowsTClause> followsTClause) {
  return dataPreprocessor->getTableByFollowsT(followsTClause);
}

shared_ptr<Table> ConcreteClauseVisitor::visit(
    shared_ptr<QB::ParentClause> parentClause) {
  return dataPreprocessor->getTableByParent(parentClause);
}

shared_ptr<Table> ConcreteClauseVisitor::visit(
    shared_ptr<QB::ParentTClause> parentTClause) {
  return dataPreprocessor->getTableByParentT(parentTClause);
}

shared_ptr<Table> ConcreteClauseVisitor::visit(
    shared_ptr<QB::UsesSClause> usesSClause) {
  return dataPreprocessor->getTableByUsesS(usesSClause);
}

shared_ptr<Table> ConcreteClauseVisitor::visit(
    shared_ptr<QB::UsesPClause> usesPClause) {
  return dataPreprocessor->getTableByUsesP(usesPClause);
}

shared_ptr<Table> ConcreteClauseVisitor::visit(
    shared_ptr<QB::ModifiesSClause> modifiesSClause) {
  return dataPreprocessor->getTableByModifiesS(modifiesSClause);
}

shared_ptr<Table> ConcreteClauseVisitor::visit(
    shared_ptr<QB::ModifiesPClause> modifiesPClause) {
  return dataPreprocessor->getTableByModifiesP(modifiesPClause);
}

shared_ptr<Table> ConcreteClauseVisitor::visit(
    shared_ptr<QB::CallsClause> callsClause) {
  return dataPreprocessor->getTableByCalls(callsClause);
}

shared_ptr<Table> ConcreteClauseVisitor::visit(
    shared_ptr<QB::CallsTClause> callsTClause) {
  return dataPreprocessor->getTableByCallsT(callsTClause);
}

shared_ptr<Table> ConcreteClauseVisitor::visit(
    shared_ptr<QB::NextClause> nextClause) {
  return dataPreprocessor->getTableByNext(nextClause);
}

shared_ptr<Table> ConcreteClauseVisitor::visit(
    shared_ptr<QB::NextTClause> nextTClause) {
  return dataPreprocessor->getTableByNextT(nextTClause);
}

shared_ptr<Table> ConcreteClauseVisitor::visit(
    shared_ptr<QB::AffectsClause> affectsClause) {
  return dataPreprocessor->getTableByAffects(affectsClause);
}

shared_ptr<Table> ConcreteClauseVisitor::visit(
    shared_ptr<QB::AffectsTClause> affectsTClause) {
  return dataPreprocessor->getTableByAffectsT(affectsTClause);
}

shared_ptr<Table> ConcreteClauseVisitor::visit(
    shared_ptr<QB::AssignPatternClause> assignPatternClause) {
  return dataPreprocessor->getTableByAssignPattern(assignPatternClause);
}
