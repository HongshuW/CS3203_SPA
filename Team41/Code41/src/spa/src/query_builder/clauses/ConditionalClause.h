//
// Created by Nafour on 4/10/22.
//

#ifndef SPA_CONDITIONALCLAUSE_H
#define SPA_CONDITIONALCLAUSE_H
#include <string>
#include <unordered_set>

#include "AbstractClause.h"

using namespace std;
class ConditionalClause : public AbstractClause {
 public:
  virtual unordered_set<string> getSynonymNames() = 0;
};

#endif  // SPA_CONDITIONALCLAUSE_H
