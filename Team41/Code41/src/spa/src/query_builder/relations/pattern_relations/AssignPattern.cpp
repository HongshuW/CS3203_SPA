//
// Created by Xingchen Lin on 1/10/22.
//

#include "AssignPattern.h"

shared_ptr<AbstractClause> AssignPattern::createClause(Synonym arg1, Ref arg2) {
    //! Partial AssignPatternClause, need to add arg3 later
    return make_shared<AssignPatternClause>(arg1, arg2);
}
