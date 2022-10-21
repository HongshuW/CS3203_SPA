//
// Created by Xingchen Lin on 30/9/22.
//

#include "NextT.h"

shared_ptr<AbstractClause> NextT::createClause(Ref arg1, Ref arg2) {
  return make_shared<NextTClause>(arg1, arg2);
}