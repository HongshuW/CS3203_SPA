//
// Created by Xingchen Lin on 28/8/22.
//

#ifndef SPA_QUERYPARSER_H
#define SPA_QUERYPARSER_H

#include <unordered_map>

#include "constants/QueryParserConstants.h"
#include "query_builder/clauses/pattern_clauses/AssignPatternClause.h"
#include "query_builder/clauses/pattern_clauses/DummyPatternClause.h"
#include "query_builder/clauses/pattern_clauses/IfPatternClause.h"
#include "query_builder/clauses/pattern_clauses/WhilePatternClause.h"
#include "query_builder/clauses/with_clauses/WithClauses.h"
#include "query_builder/commons/ExprStringTokenizer.h"
#include "query_builder/commons/ExpressionSpec.h"
#include "query_builder/commons/Query.h"
#include "query_builder/commons/Ref.h"
#include "query_builder/relations/such_that_relations/Affects.h"
#include "query_builder/relations/such_that_relations/AffectsT.h"
#include "query_builder/relations/such_that_relations/Calls.h"
#include "query_builder/relations/such_that_relations/CallsT.h"
#include "query_builder/relations/such_that_relations/Follows.h"
#include "query_builder/relations/such_that_relations/FollowsT.h"
#include "query_builder/relations/such_that_relations/Modifies.h"
#include "query_builder/relations/such_that_relations/Next.h"
#include "query_builder/relations/such_that_relations/NextT.h"
#include "query_builder/relations/such_that_relations/Parent.h"
#include "query_builder/relations/such_that_relations/ParentT.h"
#include "query_builder/relations/such_that_relations/SuchThatRelations.h"
#include "query_builder/relations/such_that_relations/Uses.h"
#include "utils/ErrorMessageFormatter.h"
#include "utils/ExprNodeParser.h"
#include "utils/Utils.h"

using namespace std;

namespace QB {
class QueryParser {
 private:
  shared_ptr<Query> query;
  int currIdx;
  vector<string> tokens;
  string peek();
  string pop();
  bool isWithinBound();
  bool expect(const string& s);
  bool match(const string& s);
  bool parseDeclarations();
  void parseSelectClause();
  Ref parseRef();
  void parseSuchThatClause();
  bool parseSuchThat();
  bool parsePattern();
  void parsePatternClause();
  ExpressionSpec parseExpressionSpec();
  void parseBooleanSelectClause();
  Elem parseTupleSelectClause();
  void parseWithClause();
  bool parseWith();
  WithRef parseWithRef();
  void validateArgRefTypeSuchThatClause(
      const shared_ptr<SuchThatClause>& suchThat) const;
  void validateArgRefTypePatternClause(const Ref& arg2) const;

 public:
  //! Parse tokens to Query object, throw Syntax Error if encounter
  explicit QueryParser(std::vector<std::string> tokens);
  shared_ptr<Query> parse();
};
}  // namespace QB

#endif  // SPA_QUERYPARSER_H