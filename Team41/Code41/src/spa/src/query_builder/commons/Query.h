//
// Created by Xingchen Lin on 28/8/22.
//

#ifndef SPA_QUERY_H
#define SPA_QUERY_H

#pragma once

#include <unordered_map>
#include <vector>

#include "Declaration.h"
#include "DesignEntity.h"
#include "Synonym.h"
#include "query_builder/clauses/pattern_clauses/PatternClause.h"
#include "query_builder/clauses/select_clauses/SelectClause.h"
#include "query_builder/clauses/such_that_clauses/SuchThatClause.h"
#include "query_builder/clauses/with_clauses/WithClauses.h"

using namespace std;

namespace QB {
class Query {
 public:
  shared_ptr<vector<Declaration>> declarations;
  shared_ptr<SelectClause> selectClause;
  shared_ptr<vector<shared_ptr<SuchThatClause>>> suchThatClauses;
  shared_ptr<vector<shared_ptr<PatternClause>>> patternClauses;
  shared_ptr<vector<shared_ptr<WithClause>>> withClauses;
  Query();
};
}  // namespace QB

#endif  // SPA_QUERY_H
