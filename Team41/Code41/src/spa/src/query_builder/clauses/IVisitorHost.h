//
// Created by Nafour on 30/9/22.
//

#ifndef SPA_IVISITORHOST_H
#define SPA_IVISITORHOST_H


#include "pkb/table/Table.h"

class IVisitor;

class IVisitorHost {
public:
    virtual ~IVisitorHost() = default;
    virtual Table accept(shared_ptr<IVisitor> visitor) = 0;
};


#endif //SPA_IVISITORHOST_H
