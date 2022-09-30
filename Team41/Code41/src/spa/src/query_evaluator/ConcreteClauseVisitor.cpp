//
// Created by Nafour on 30/9/22.
//

#include "ConcreteClauseVisitor.h"
#include "query_builder/clauses/pattern_clauses/AssignPatternClause.h"

Table ConcreteClauseVisitor::visit(shared_ptr<QB::IfPatternClause> ifPatternClause) {
    cout << "visit if pattern" << endl;
    return Table();
}

ConcreteClauseVisitor::ConcreteClauseVisitor(shared_ptr<QE::DataPreprocessor> dataPreprocessor) : AbstractClauseVisitor(
        dataPreprocessor) {

}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::WhilePatternClause> WhilePatternClause) {
    cout << "visit while pattern" << endl;
    return Table();
}

Table ConcreteClauseVisitor::visit(shared_ptr<QB::AssignPatternClause> assignPatternClause) {
    cout << "visit assign pattern" << endl;
    Table table = dataPreprocessor->getTableByExprPattern(assignPatternClause->arg3);
    return dataPreprocessor->filterSingleClauseResultTable(assignPatternClause->arg1, assignPatternClause->arg2, table);
}
