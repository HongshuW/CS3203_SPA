//
// Created by Xingchen Lin on 28/8/22.
//

#include "Query.h"
#include "Synonym.h"

using namespace QB;

Query::Query()
    : declarations(new std::vector<Declaration>),
      clauses(new std::vector<Clause *>){};

Query::~Query() {
    delete declarations;
    delete clauses;
}
