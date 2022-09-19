//
// Created by Xingchen Lin on 29/8/22.
//

#include "SelectClause.h"

using namespace QB;

SelectClause::SelectClause(ReturnType returnType) : returnType(returnType){};

SelectClause::SelectClause(ReturnType returnType, shared_ptr<vector<Elem>> returnResults) :
        returnType(returnType), returnResults(returnResults) {};

bool SelectClause::isBoolean() {
    return returnType == ReturnType::BOOLEAN;
}

bool SelectClause::isTuple() {
    return returnType == ReturnType::TUPLE;
}