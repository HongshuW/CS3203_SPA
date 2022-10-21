//
// Created by Xingchen Lin on 1/10/22.
//

#include "IfPattern.h"

shared_ptr<AbstractClause> IfPattern::createClause(Synonym arg1, Ref arg2) {
  return make_shared<IfPatternClause>(arg1, arg2);
}
