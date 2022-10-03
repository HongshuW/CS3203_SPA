//
// Created by Nafour on 30/9/22.
//

#include "ConcreteClauseVisitor.h"
#include "query_builder/clauses/pattern_clauses/AssignPatternClause.h"
#include "query_builder/clauses/such_that_clauses/FollowsClause.h"
#include "query_builder/clauses/pattern_clauses/IfPatternClause.h"
ConcreteClauseVisitor::ConcreteClauseVisitor(shared_ptr<QE::DataPreprocessor> dataPreprocessor) : AbstractClauseVisitor(
        dataPreprocessor) {

}

//only for no condition select clause
Table ConcreteClauseVisitor::visit(shared_ptr<QB::SelectClause> selectClause) {
    return dataPreprocessor->getNoConditionSelectClauseResult(selectClause);
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::IfPatternClause> ifPatternClause) {
    return dataPreprocessor->getIfPatternTable(ifPatternClause);
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::WhilePatternClause> whilePatternClause) {
    return dataPreprocessor->getWhilePatternTable(whilePatternClause);
}

//! dummy, should be called
Table ConcreteClauseVisitor::visit(shared_ptr<QB::InvalidPatternClause> invalidPatternClause) {
    return Table();
}


Table ConcreteClauseVisitor::visit(shared_ptr<QB::WithClause> withClause) {
    return dataPreprocessor->getTableByWith(withClause);
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::FollowsClause> followsClause) {
    return dataPreprocessor->getTableByFollows(followsClause);
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::FollowsTClause> followsTClause) {
    return dataPreprocessor->getTableByFollowsT(followsTClause);
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::ParentClause> parentClause) {
    return dataPreprocessor->getTableByParent(parentClause);
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::ParentTClause> parentTClause) {
    return dataPreprocessor->getTableByParentT(parentTClause);
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::UsesSClause> usesSClause) {
    return dataPreprocessor->getTableByUsesS(usesSClause);
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::UsesPClause> usesPClause) {
    return dataPreprocessor->getTableByUsesP(usesPClause);
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::ModifiesSClause> modifiesSClause) {
    return dataPreprocessor->getTableByModifiesS(modifiesSClause);
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::ModifiesPClause> modifiesPClause) {
    return dataPreprocessor->getTableByModifiesP(modifiesPClause);
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::CallsClause> callsClause) {
    return dataPreprocessor->getTableByCalls(callsClause);
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::CallsTClause> callsTClause) {
    return dataPreprocessor->getTableByCallsT(callsTClause);
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::NextClause> nextClause) {
    return dataPreprocessor->getTableByNext(nextClause);
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::NextTClause> nextTClause) {
    return dataPreprocessor->getTableByNextT(nextTClause);
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::AffectsClause> affectsClause) {
    return dataPreprocessor->getTableByAffects(affectsClause);
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::AffectsTClause> affectsTClause) {
    return dataPreprocessor->getTableByAffectsT(affectsTClause);
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::AssignPatternClause> assignPatternClause) {
    return dataPreprocessor->getTableByAssignPattern(
            assignPatternClause);
}
