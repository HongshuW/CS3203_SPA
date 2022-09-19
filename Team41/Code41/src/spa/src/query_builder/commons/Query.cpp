//
// Created by Xingchen Lin on 28/8/22.
//

#include "Query.h"
#include "Synonym.h"

using namespace QB;

Query::Query()
        : declarations(make_shared<vector<Declaration>>()),
          suchThatClauses(make_shared<vector<shared_ptr<SuchThatClause>>>()),
          patternClause(make_shared<vector<shared_ptr<PatternClause>>>()){}