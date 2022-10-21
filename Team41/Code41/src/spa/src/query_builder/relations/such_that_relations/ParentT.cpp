//
// Created by Xingchen Lin on 30/9/22.
//

#include "ParentT.h"

shared_ptr<AbstractClause> ParentT::createClause(Ref arg1, Ref arg2) {
  return make_shared<ParentTClause>(arg1, arg2);
}