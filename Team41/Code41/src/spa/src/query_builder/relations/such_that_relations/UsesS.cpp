//
// Created by Xingchen Lin on 30/9/22.
//

#include "UsesS.h"

shared_ptr<AbstractClause> UsesS::createClause(Ref arg1, Ref arg2) {
  return make_shared<UsesSClause>(arg1, arg2);
}