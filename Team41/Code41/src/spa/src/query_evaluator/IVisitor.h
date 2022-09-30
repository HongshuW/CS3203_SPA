//
// Created by Nafour on 30/9/22.
//

#ifndef SPA_IVISITOR_H
#define SPA_IVISITOR_H

#include "pkb/table/Table.h"
namespace QB {
    class IfPatternClause;
    class WhilePatternClause;
    class AssignPatternClause;
}

class IVisitor {
public:
    virtual ~IVisitor() = default;
    virtual Table visit(shared_ptr<QB::IfPatternClause> ifPatternClause) = 0;
    virtual Table visit(shared_ptr<QB::WhilePatternClause> WhilePatternClause) = 0;
    virtual Table visit(shared_ptr<QB::AssignPatternClause> AssignPatternClause) = 0;

};


#endif //SPA_IVISITOR_H
