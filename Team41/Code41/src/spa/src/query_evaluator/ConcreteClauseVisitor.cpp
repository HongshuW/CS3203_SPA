//
// Created by Nafour on 30/9/22.
//

#include "ConcreteClauseVisitor.h"
#include "query_builder/clauses/pattern_clauses/AssignPatternClause.h"

ConcreteClauseVisitor::ConcreteClauseVisitor(shared_ptr<QE::DataPreprocessor> dataPreprocessor) : AbstractClauseVisitor(
        dataPreprocessor) {

}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::SelectClause> selectClause) {
    return Table();
}


Table ConcreteClauseVisitor::visit(shared_ptr<QB::IfPatternClause> ifPatternClause) {
    return Table();
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::WhilePatternClause> whilePatternClause) {
    return Table();
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::WithClause> withClause) {
    return Table();
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::FollowsClause> followsClause) {
    return Table();
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::FollowsTClause> followsTClause) {
    return Table();
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::ParentClause> parentClause) {
    return Table();
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::ParentTClause> parentTClause) {
    return Table();
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::UsesSClause> usesSClause) {
    return Table();
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::UsesPClause> usesPClause) {
    return Table();
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::ModifiesSClause> modifiesSClause) {
    return Table();
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::ModifiesPClause> modifiesPClause) {
    return Table();
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::CallsClause> callsClause) {
    return Table();
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::CallsTClause> callsTClause) {
    return Table();
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::NextClause> nextClause) {
    return Table();
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::NextTClause> nextTClause) {
    return Table();
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::AffectsClause> affectsClause) {
    return Table();
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::AffectsTClause> affectsTClause) {
    return Table();
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::AssignPatternClause> assignPatternClause) {
    cout << "visit assign pattern" << endl;
    Table table = dataPreprocessor->getTableByExprPattern(assignPatternClause->arg3);
    return dataPreprocessor->filterSingleClauseResultTable(assignPatternClause->arg1, assignPatternClause->arg2, table);
}
