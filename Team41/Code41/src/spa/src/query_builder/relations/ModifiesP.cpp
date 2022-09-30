//
// Created by Xingchen Lin on 30/9/22.
//

#include "ModifiesP.h"

shared_ptr<AbstractClause> ModifiesP::createClause(Ref arg1, Ref arg2) {
    return make_shared<ModifiesPClause>(arg1, arg2);
}