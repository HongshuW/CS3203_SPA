//
// Created by Nafour on 4/10/22.
//

#include "QueryOptimizer.h"

#include <set>
#include <utility>

#include "query_builder/clauses/pattern_clauses/AssignPatternClause.h"
#include "query_builder/clauses/pattern_clauses/IfPatternClause.h"
#include "query_builder/clauses/pattern_clauses/WhilePatternClause.h"
#include "query_builder/clauses/such_that_clauses/AffectsClause.h"
#include "query_builder/clauses/such_that_clauses/AffectsTClause.h"
#include "query_builder/clauses/such_that_clauses/CallsTClause.h"
#include "query_builder/clauses/such_that_clauses/FollowsClause.h"
#include "query_builder/clauses/such_that_clauses/FollowsTClause.h"
#include "query_builder/clauses/such_that_clauses/ModifiesPClause.h"
#include "query_builder/clauses/such_that_clauses/ModifiesSClause.h"
#include "query_builder/clauses/such_that_clauses/NextClause.h"
#include "query_builder/clauses/such_that_clauses/NextTClause.h"
#include "query_builder/clauses/such_that_clauses/ParentClause.h"
#include "query_builder/clauses/such_that_clauses/ParentTClause.h"
#include "query_builder/clauses/such_that_clauses/UsesPClause.h"
#include "query_builder/clauses/with_clauses/WithClauses.h"

namespace QE {
const int QueryOptimizer::NO_SYN_GROUP_IDX = -1;
const vector<string> lowCostRelations = {
    typeid(FollowsClause).name(),
    typeid(WithClause).name(),
    typeid(NextClause).name(),
};

const vector<string> highCostRelations = {
    typeid(AffectsTClause).name(),  typeid(FollowsTClause).name(),
    typeid(ParentTClause).name(),   typeid(CallsTClause).name(),
    typeid(ModifiesPClause).name(), typeid(UsesPClause).name()};
const int DEFAULT_COST = 100;
const int SMALL_COST_MAGNITUDE = 5;
const int MEDIUM_COST_MAGNITUDE = 10;
const int LARGE_COST_MAGNITUDE = 15;
QueryOptimizer::QueryOptimizer(shared_ptr<Query> query) : query(std::move(query)) {
	clauseCount = 0;
  initMaps();
  initParent();

  initEdges();
  processEdges();
}

ConnectedClauseGroups QueryOptimizer::optimise() {
  return optimiseSubGroups(getConnectedGroups());
}

void QueryOptimizer::initParent() {
  parent = vector<int>(clauseCount);
  for (int i = 0; i < clauseCount; i++) {
    parent[i] = i;
  }
}

void QueryOptimizer::initMaps() {
  int clauseId = 0;
  for (const auto& clause : *query->suchThatClauses) {
    clauseIdMap.insert({clause, clauseId});
    idClauseMap.insert({clauseId, clause});
    clauseId++;
  }
  for (const auto& clause : *query->patternClauses) {
    clauseIdMap.insert({clause, clauseId});
    idClauseMap.insert({clauseId, clause});
    clauseId++;
  }
  for (const auto& clause : *query->withClauses) {
    clauseIdMap.insert({clause, clauseId});
    idClauseMap.insert({clauseId, clause});
    clauseId++;
  }
  clauseCount = (int) clauseIdMap.size();
}

void QueryOptimizer::initEdges() {
  size_t M = clauseIdMap.size();
  const bool value = false;
  const int valueInt = 0;
  vector<bool> row(M, value);
  vector<vector<bool>> visited(M, row);

  edgeWeights.resize(M, vector<int>(M, valueInt));

  for (auto& it_i : clauseIdMap) {
    auto curr_i = it_i.first;
    for (auto& it_j : clauseIdMap) {
      auto curr_j = it_j.first;
      if (curr_i == curr_j || !hasCommonSyn(curr_i, curr_j) ||
          visited[clauseIdMap.at(curr_i)][clauseIdMap.at(curr_j)])
        continue;

      visited[clauseIdMap.at(curr_i)][clauseIdMap.at(curr_j)] = true;
      edges.emplace_back(clauseIdMap.at(curr_i), clauseIdMap.at(curr_j));
      edgeWeights[clauseIdMap.at(curr_i)][clauseIdMap.at(curr_j)] =
          calculateEdgeWeight(curr_i, curr_j);
    }
  }
}

bool QueryOptimizer::hasCommonSyn(const shared_ptr<ConditionalClause>& clause1,
                                  const shared_ptr<ConditionalClause>& clause2) {
  auto set1 = clause1->getSynonymNames();
  for (const auto& syn : clause2->getSynonymNames()) {
    if (set1.count(syn)) return true;
  }
  return false;
}

int QueryOptimizer::calculateEdgeWeight(const shared_ptr<ConditionalClause>& clause1,
                                        const shared_ptr<ConditionalClause>& clause2) {
  int curr_cost = DEFAULT_COST;
  const int SYN_COUNT_THRESHOLD = 1;
  const int IDENT_OR_INT_COUNT_THRESHOLD = 1;
  // reduce cost for clauses with only one synonym
  if (clause1->getSynonymNames().size() <= SYN_COUNT_THRESHOLD) {
    curr_cost -= SMALL_COST_MAGNITUDE;
    // reduce cost for clauses with an ident/int value since filtering can be
    // done to reduce table size
    if (clause1->getValueRefCount() >= IDENT_OR_INT_COUNT_THRESHOLD)
      curr_cost -= SMALL_COST_MAGNITUDE;
  }
  if (clause2->getSynonymNames().size() <= SYN_COUNT_THRESHOLD) {
    curr_cost -= SMALL_COST_MAGNITUDE;
    if (clause2->getValueRefCount() >= IDENT_OR_INT_COUNT_THRESHOLD)
      curr_cost -= SMALL_COST_MAGNITUDE;
  }

  // reduce cost for clauses with lesser number of results
  if (std::find(lowCostRelations.begin(), lowCostRelations.end(),
                typeid(*clause1).name()) != lowCostRelations.end()) {
    curr_cost -= MEDIUM_COST_MAGNITUDE;
  }
  if (std::find(lowCostRelations.begin(), lowCostRelations.end(),
                typeid(*clause2).name()) != lowCostRelations.end()) {
    curr_cost -= MEDIUM_COST_MAGNITUDE;
  }

  // increase cost for clauses with high number of results
  if (std::find(highCostRelations.begin(), highCostRelations.end(),
                typeid(*clause1).name()) != highCostRelations.end()) {
    curr_cost += LARGE_COST_MAGNITUDE;
  }
  if (std::find(highCostRelations.begin(), highCostRelations.end(),
                typeid(*clause2).name()) != highCostRelations.end()) {
    curr_cost += LARGE_COST_MAGNITUDE;
  }

  return curr_cost;
}

int QueryOptimizer::getLowerCostClause(int x, int y) {
  auto clause1 = idClauseMap.at(x);
  auto clause2 = idClauseMap.at(y);
  int cl1Cost = DEFAULT_COST;
  int cl2Cost = DEFAULT_COST;

  const int SYN_COUNT_THRESHOLD = 1;
  const int IDENT_OR_INT_COUNT_THRESHOLD = 1;

  if (clause1->getSynonymNames().size() <= SYN_COUNT_THRESHOLD) {
    cl1Cost -= SMALL_COST_MAGNITUDE;
    if (clause1->getValueRefCount() >= IDENT_OR_INT_COUNT_THRESHOLD)
      cl1Cost -= SMALL_COST_MAGNITUDE;
  }
  if (clause2->getSynonymNames().size() <= SYN_COUNT_THRESHOLD) {
    cl2Cost -= SMALL_COST_MAGNITUDE;
    if (clause2->getValueRefCount() >= IDENT_OR_INT_COUNT_THRESHOLD)
      cl2Cost -= SMALL_COST_MAGNITUDE;
  }

  // reduce cost for clauses with lesser number of results
  if (std::find(lowCostRelations.begin(), lowCostRelations.end(),
                typeid(*clause1).name()) != lowCostRelations.end()) {
    cl1Cost -= MEDIUM_COST_MAGNITUDE;
  }
  if (std::find(lowCostRelations.begin(), lowCostRelations.end(),
                typeid(*clause2).name()) != lowCostRelations.end()) {
    cl2Cost -= MEDIUM_COST_MAGNITUDE;
  }

  // increase cost for clauses with high number of results
  if (std::find(highCostRelations.begin(), highCostRelations.end(),
                typeid(*clause1).name()) != highCostRelations.end()) {
    cl1Cost += LARGE_COST_MAGNITUDE;
  }
  if (std::find(highCostRelations.begin(), highCostRelations.end(),
                typeid(*clause2).name()) != highCostRelations.end()) {
    cl2Cost += LARGE_COST_MAGNITUDE;
  }

  return cl1Cost > cl2Cost ? y : x;
}

int QueryOptimizer::root(int clause) {
  if (clause == parent[clause]) {
    return clause;
  }
  return parent[clause] = root(parent[clause]);
}

int QueryOptimizer::connect(int clause1, int clause2) {
  clause1 = root(clause1);
  clause2 = root(clause2);
  if (clause1 != clause2) {
    parent[clause2] = clause1;
  }
  return clause1;
}

void QueryOptimizer::processEdges() {
  for (auto edge : edges) {
    connect(edge.first, edge.second);
  }
}

ConnectedClauseGroups QueryOptimizer::getConnectedGroups() {
  ConnectedClauseGroups ccg = make_shared<
      unordered_map<int, shared_ptr<vector<shared_ptr<ConditionalClause>>>>>();
  ccg->insert(
      {NO_SYN_GROUP_IDX, make_shared<vector<shared_ptr<ConditionalClause>>>()});

  for (int i = 0; i < clauseCount; i++) {
    // clauses without synonyms are grouped together
    if (idClauseMap.at(i)->getSynonymNames().empty()) {
      ccg->at(NO_SYN_GROUP_IDX)->push_back(idClauseMap.at(i));
      continue;
    }

    int currRoot = root(parent[i]);
    if (ccg->find(currRoot) == ccg->end()) {
      ccg->insert(
          {currRoot, make_shared<vector<shared_ptr<ConditionalClause>>>()});
    }

    ccg->at(currRoot)->push_back(idClauseMap.at(i));
  }

  return ccg;
}

ConnectedClauseGroups QueryOptimizer::optimiseSubGroups(
    ConnectedClauseGroups ccg) {
  // sort
  for (auto& it : *ccg) {
    if (it.first == NO_SYN_GROUP_IDX) continue;
    shared_ptr<vector<shared_ptr<ConditionalClause>>> sortedVec =
        make_shared<vector<shared_ptr<ConditionalClause>>>();
    auto clauseGroupVec = it.second;
    const int V = (int) clauseGroupVec->size();
    vector<bool> selected(clauseCount, false);
    int no_edge = 0;

    // pick first vertex with min edge cost:
    int first = getMinClauseIdFromGroup(clauseGroupVec);
    sortedVec->push_back(idClauseMap.at(first));
    selected[first] = true;

    // prims algo to sort vectors and ensure clauses with connected synonyms
    // are evaluated together
    int y;
    while (no_edge < V - 1) {
      int min = INF;
      y = 0;

      for (const auto& cl1 : *clauseGroupVec) {
        int i = clauseIdMap.at(cl1);
        if (!selected[i]) continue;
        for (const auto& cl2 : *clauseGroupVec) {
          int j = clauseIdMap.at(cl2);
          if (selected[j] || !edgeWeights[i][j]) continue;
          if (min < edgeWeights[i][j]) continue;
          min = edgeWeights[i][j];
          y = j;
        }
      }
      sortedVec->push_back(idClauseMap.at(y));
      selected[y] = true;
      no_edge++;
    }
    it.second = sortedVec;
  }
  return ccg;
}

int QueryOptimizer::getMinClauseIdFromGroup(const SubgroupClauses& clauses) {
  if (clauses->size() == 1) return clauseIdMap.at(clauses->at(0));
  // pick first vertex with min edge cost:
  int minEdgeCost = INF;
  int x_first = 0;
  int y_first = 0;
  for (const auto& cl1 : *clauses) {
    int i = clauseIdMap.at(cl1);
    for (const auto& cl2 : *clauses) {
      if (cl1 == cl2) continue;
      int j = clauseIdMap.at(cl2);
      if (!edgeWeights[i][j] || minEdgeCost < edgeWeights[i][j]) continue;
      minEdgeCost = edgeWeights[i][j];
      x_first = i;
      y_first = j;
    }
  }
  int first = getLowerCostClause(x_first, y_first);
  return first;
}

}  // namespace QE