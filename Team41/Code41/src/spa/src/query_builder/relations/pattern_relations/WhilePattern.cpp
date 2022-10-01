//
// Created by Xingchen Lin on 1/10/22.
//

#include "WhilePattern.h"

shared_ptr<AbstractClause> WhilePattern::createClause(Synonym arg1, Ref arg2) {
    return make_shared<WhilePatternClause>(arg1, arg2);
}
