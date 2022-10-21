//
// Created by Xingchen Lin on 30/9/22.
//

#include "Calls.h"

shared_ptr<AbstractClause> Calls::createClause(Ref arg1, Ref arg2) {
  return make_shared<CallsClause>(arg1, arg2);
}