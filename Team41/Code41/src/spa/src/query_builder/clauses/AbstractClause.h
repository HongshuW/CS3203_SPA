//
// Created by Nafour on 30/9/22.
//

#ifndef SPA_ABSTRACTCLAUSE_H
#define SPA_ABSTRACTCLAUSE_H
#include "IVisitorHost.h"

class AbstractClause: public IVisitorHost {

    virtual Table accept(shared_ptr<IVisitor> visitor) = 0;
};


#endif //SPA_ABSTRACTCLAUSE_H
