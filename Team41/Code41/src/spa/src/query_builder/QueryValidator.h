//
// Created by Xingchen Lin on 31/8/22.
//

#ifndef SPA_QUERYVALIDATOR_H
#define SPA_QUERYVALIDATOR_H

#pragma once

#include <unordered_set>

#include "query_builder/clauses/such_that_clauses/ModifiesPClause.h"
#include "query_builder/clauses/such_that_clauses/ModifiesSClause.h"
#include "query_builder/clauses/such_that_clauses/UsesPClause.h"
#include "query_builder/clauses/such_that_clauses/UsesSClause.h"
#include "query_builder/clauses/such_that_clauses/Validatable.h"
#include "query_builder/clauses/pattern_clauses/DummyPatternClause.h"
#include "query_builder/commons/Query.h"
#include "query_builder/commons/WithRef.h"
#include "query_builder/constants/QueryValidatorConstants.h"
#include "query_builder/relations/pattern_relations/PatternRelations.h"
#include "query_builder/relations/pattern_relations/AssignPattern.h"
#include "query_builder/relations/pattern_relations/IfPattern.h"
#include "query_builder/relations/pattern_relations/WhilePattern.h"

using namespace std;

namespace QB {
class QueryValidator {
 private:
  shared_ptr<Query> query;
  void validateNoDuplicateDeclarations() const;
  void validateSelectClause();
  void validateSynonymDeclaredSelectClause() const;
  void validateDesignEntityAttrNamePairSelectClause() const;
  void validateSynonymDeclaredSuchThatClause();
  void checkSynonymDeclared(
      Synonym& synonym, const shared_ptr<vector<Declaration>>& declarations);
  void validateArgRefTypeSuchThatClause() const;
  void validateUsesModifiesNoUnderscoreForFirstArg() const;
  void validateSynonymTypeSuchThatClause();
  void validateSuchThatClause();
  void checkCorrectDesignEntity(
      Synonym synonym, const unordered_set<DesignEntity>& validSynonymTypes,
      const shared_ptr<vector<Declaration>>& declarations);
  void validateSynonymDeclaredPatternClause();
  void validateAllowedDesignEntityPatternClause();
  void validateArgRefTypePatternClause() const;
  void validateArg2DesignEntityPatternClause() const;
  void validatePatternClause();
  void checkDesignEntityAttrNamePair(
      WithRef& withRef, const shared_ptr<vector<Declaration>>& declarations);
  void validateDesignEntityAttrNamePairWithClause();
  void validateSameWithRefWithClause();
  WithComparingType getWithComparingType(
      WithRef withRef, const shared_ptr<WithClause>& withClause);
  void validateWithClause();

 public:
  //! Validate the Query object, throw Semantic Error if encounter
  explicit QueryValidator(shared_ptr<Query> query);
  void validateQuery();
};
}  // namespace QB

#endif  // SPA_QUERYVALIDATOR_H
