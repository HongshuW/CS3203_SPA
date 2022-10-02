//
// Created by Xingchen Lin on 30/9/22.
//

#include "Next.h"

shared_ptr<AbstractClause> Next::createClause(Ref arg1, Ref arg2) {
    return make_shared<NextClause>(arg1, arg2);
}