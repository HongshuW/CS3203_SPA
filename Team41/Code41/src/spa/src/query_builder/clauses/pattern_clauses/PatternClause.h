//
// Created by Xingchen Lin on 11/9/22.
//

#ifndef SPA_PATTERNCLAUSE_H
#define SPA_PATTERNCLAUSE_H

#pragma once
#include "query_builder/clauses/AbstractClause.h"
#include "query_builder/clauses/ConditionalClause.h"
#include "query_builder/commons/Ref.h"
#include "query_builder/commons/Synonym.h"
#include "utils/ErrorMessageFormatter.h"
using namespace QB;

namespace QB {
class PatternClause : public ConditionalClause {
 public:
  Synonym arg1;  // must be declared as If
  Ref arg2;      // entRef -> synonym, _, ident
  PatternClause(Synonym arg1, Ref arg2);
  ~PatternClause() override = default;
  virtual Table accept(shared_ptr<IVisitor> visitor) override = 0;
  virtual int validateSyntaxError(int currIdx,
                                  const vector<string>& tokens) = 0;
  virtual bool expect(const string& s, unsigned int currIdx,
                      const vector<string>& tokens);
  unordered_set<string> getSynonymNames() override;
  int getValueRefCount() override;
};
}  // namespace QB

#endif  // SPA_PATTERNCLAUSE_H
