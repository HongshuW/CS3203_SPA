//
// Created by Xingchen Lin on 30/9/22.
//

#include "CallsT.h"

shared_ptr<AbstractClause> CallsT::createClause(Ref arg1, Ref arg2) {
  return make_shared<CallsTClause>(arg1, arg2);
}