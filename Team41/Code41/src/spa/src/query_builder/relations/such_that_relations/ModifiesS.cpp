//
// Created by Xingchen Lin on 30/9/22.
//

#include "ModifiesS.h"

shared_ptr<AbstractClause> ModifiesS::createClause(Ref arg1, Ref arg2) {
    return make_shared<ModifiesSClause>(arg1, arg2);
}