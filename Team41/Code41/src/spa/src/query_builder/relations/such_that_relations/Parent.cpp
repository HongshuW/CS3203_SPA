//
// Created by Xingchen Lin on 30/9/22.
//

#include "Parent.h"

shared_ptr<AbstractClause> Parent::createClause(Ref arg1, Ref arg2) {
    return make_shared<ParentClause>(arg1, arg2);
}