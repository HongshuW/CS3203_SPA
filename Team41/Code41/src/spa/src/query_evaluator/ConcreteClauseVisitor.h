//
// Created by Nafour on 30/9/22.
//

#ifndef SPA_CONCRETECLAUSEVISITOR_H
#define SPA_CONCRETECLAUSEVISITOR_H


#include "AbstractClauseVisitor.h"
class ConcreteClauseVisitor: public AbstractClauseVisitor {


public:
    explicit ConcreteClauseVisitor(shared_ptr<QE::DataPreprocessor> dataPreprocessor);
    Table visit(shared_ptr<QB::IfPatternClause> ifPatternClause) override;
    Table visit(shared_ptr<QB::WhilePatternClause> WhilePatternClause) override;
    Table visit(shared_ptr<QB::AssignPatternClause> assignPatternClause) override;

};


#endif //SPA_CONCRETECLAUSEVISITOR_H
