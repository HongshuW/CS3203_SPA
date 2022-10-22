//
// Created by Nafour on 4/10/22.
//

#ifndef SPA_QUERYOPTIMIZER_H
#define SPA_QUERYOPTIMIZER_H
#include <unordered_set>

#include "query_builder/clauses/ConditionalClause.h"
#include "query_builder/commons/Query.h"
#define INF 9999999
using namespace std;
namespace QE {
typedef shared_ptr<
    unordered_map<int, shared_ptr<vector<shared_ptr<ConditionalClause>>>>>
    ConnectedClauseGroups;
typedef shared_ptr<vector<shared_ptr<ConditionalClause>>> SubgroupClauses;
class QueryOptimizer {
  shared_ptr<Query> query;
  int clauseCount;
  vector<int> parent;
  vector<pair<int, int>> edges;
  unordered_map<shared_ptr<ConditionalClause>, int> clauseIdMap;
  unordered_map<int, shared_ptr<ConditionalClause>> idClauseMap;
  vector<vector<int>> edgeWeights;

  int getMinClauseIdFromGroup(SubgroupClauses clauses);

  void initParent();
  void initMaps();
  void initEdges();

  bool hasCommonSyn(shared_ptr<ConditionalClause> clause1,
                    shared_ptr<ConditionalClause> clause2);
  int calculateEdgeWeight(shared_ptr<ConditionalClause> clause1,
                          shared_ptr<ConditionalClause> clause2);
  int getLowerCostClause(int x, int y);

  int root(int clause);
  int connect(int clause1, int clause2);

  void processEdges();
  ConnectedClauseGroups getConnectedGroups();
  ConnectedClauseGroups optimiseSubGroups(ConnectedClauseGroups ccg);

 public:
  explicit QueryOptimizer(shared_ptr<Query> query);
  ConnectedClauseGroups optimise();
  static const int NO_SYN_GROUP_IDX;
};

}  // namespace QE

#endif  // SPA_QUERYOPTIMIZER_H
