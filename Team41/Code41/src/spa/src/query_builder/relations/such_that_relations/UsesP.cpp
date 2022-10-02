//
// Created by Xingchen Lin on 30/9/22.
//

#include "UsesP.h"

shared_ptr<AbstractClause> UsesP::createClause(Ref arg1, Ref arg2) {
    return make_shared<UsesPClause>(arg1, arg2);
}