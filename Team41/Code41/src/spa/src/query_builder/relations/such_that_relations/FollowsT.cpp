//
// Created by Xingchen Lin on 30/9/22.
//

#include "FollowsT.h"

shared_ptr<AbstractClause> FollowsT::createClause(Ref arg1, Ref arg2) {
  return make_shared<FollowsTClause>(arg1, arg2);
}