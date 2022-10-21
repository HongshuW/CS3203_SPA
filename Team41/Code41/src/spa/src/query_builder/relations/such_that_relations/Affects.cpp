//
// Created by Xingchen Lin on 30/9/22.
//

#include "Affects.h"

shared_ptr<AbstractClause> Affects::createClause(Ref arg1, Ref arg2) {
  return make_shared<AffectsClause>(arg1, arg2);
}