//
// Created by Xingchen Lin on 30/9/22.
//

#include "Follows.h"
#include <memory>

shared_ptr<AbstractClause> Follows::createClause(Ref arg1, Ref arg2) {
    return make_shared<FollowsClause>(arg1, arg2);
}