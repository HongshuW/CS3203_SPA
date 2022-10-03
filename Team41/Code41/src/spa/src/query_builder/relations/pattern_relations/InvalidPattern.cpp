//
// Created by Xingchen Lin on 3/10/22.
//

#include "InvalidPattern.h"

shared_ptr<AbstractClause> InvalidPattern::createClause(Synonym arg1, Ref arg2) {
    return make_shared<InvalidPatternClause>(arg1, arg2);
}