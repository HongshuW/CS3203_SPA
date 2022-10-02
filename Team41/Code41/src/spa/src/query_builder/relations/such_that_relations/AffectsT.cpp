//
// Created by Xingchen Lin on 30/9/22.
//

#include "AffectsT.h"

shared_ptr<AbstractClause> AffectsT::createClause(Ref arg1, Ref arg2) {
    return make_shared<AffectsTClause>(arg1, arg2);
}