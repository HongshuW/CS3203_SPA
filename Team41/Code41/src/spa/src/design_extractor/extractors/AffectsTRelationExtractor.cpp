//
// Created by Nafour on 29/10/22.
//

#include "AffectsTRelationExtractor.h"

#include "design_extractor/results/QueryTimeResult.h"
#include "design_extractor/results/RelationResult.h"

namespace DE {
AffectsTRelationExtractor::AffectsTRelationExtractor(
    shared_ptr<ProgramNode> programNode)
    : AffectsCommonExtractor(programNode) {
  programSize = stmtNumbers->size();
  vector<bool> row(programSize, false);
  dp = vector<vector<bool>>(programSize, row);
  generateAffectsTable();
}

shared_ptr<ExtractorResult> AffectsTRelationExtractor::extract(
    StmtNoArgs args) {
  if (args.startAndEndExists()) {
    if (affectsAdjListPtr == nullptr) initAffectsAdjList();
    return make_shared<QueryTimeResult>(
        extractNoWildcard(args.getStartStmtNo(), args.getEndStmtNo()));
  }
  if (args.startExistsOnly()) {
    if (affectsAdjListPtr == nullptr) initAffectsAdjList();
    return make_shared<QueryTimeResult>(
        extractWithStartGiven(args.getStartStmtNo()));
  }
  if (args.endExistsOnly()) {
    if (affectsAdjListReversedPtr == nullptr) initAffectsAdjListReversed();
    return make_shared<QueryTimeResult>(
        extractWithEndGiven(args.getEndStmtNo()));
  }
  return make_shared<QueryTimeResult>(make_shared<vector<string>>());
}

shared_ptr<ExtractorResult> AffectsTRelationExtractor::extractAllRelations() {
  const int EDGE_INDEX_OFFSET = 1;
  for (auto &relation : *affectsTable) {
    int i = stoi(relation[0]) - EDGE_INDEX_OFFSET;
    int j = stoi(relation[1]) - EDGE_INDEX_OFFSET;
    dp[i][j] = true;
  }
  const int n = programSize;
  for (int k = 0; k < n; k++) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        dp[i][j] = dp[i][j] || (dp[i][k] && dp[k][j]);
      }
    }
  }
  shared_ptr<list<vector<string>>> output = make_shared<list<vector<string>>>();
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (!dp[i][j]) continue;
      output->push_back(
          {to_string(i + EDGE_INDEX_OFFSET), to_string(j + EDGE_INDEX_OFFSET)});
    }
  }
  return make_shared<RelationResult>(output);
}

void AffectsTRelationExtractor::initAffectsAdjList() {
  affectsAdjListPtr = make_shared<unordered_map<int, unordered_set<int>>>();
  for (int i = 0; i < programSize; i++) {
    affectsAdjListPtr->insert({i + 1, unordered_set<int>()});
  }
  for (auto &relation : *affectsTable) {
    affectsAdjListPtr->at(stoi(relation[0])).insert(stoi(relation[1]));
  }
}

shared_ptr<vector<string>> AffectsTRelationExtractor::extractNoWildcard(
    int start, int end) {
  bool isValid =
      extractNoWildcardDFS(start, end, make_shared<unordered_set<int>>());
  auto output = make_shared<vector<string>>();
  if (!isValid) return output;
  output->push_back(to_string(start));
  output->push_back(to_string(end));
  return output;
}

bool AffectsTRelationExtractor::extractNoWildcardDFS(
    int curr, int target, shared_ptr<unordered_set<int>> visited) {
  visited->insert(curr);
  bool isFound = false;
  for (auto &neighbor : affectsAdjListPtr->at(curr)) {
    if (neighbor == target) return true;
    if (visited->count(neighbor)) continue;
    isFound |= extractNoWildcardDFS(neighbor, target, visited);
  }
  return isFound;
}

shared_ptr<vector<string>> AffectsTRelationExtractor::extractWithStartGiven(
    int start) {
  shared_ptr<vector<string>> result = make_shared<vector<string>>();
  extractWithStartGivenDFS(start, start, result,
                           make_shared<unordered_set<int>>());
  return result;
}

void AffectsTRelationExtractor::extractWithStartGivenDFS(
    int curr, int start, shared_ptr<vector<string>> result,
    shared_ptr<unordered_set<int>> visited) {
  visited->insert(curr);
  for (auto &neighbor : affectsAdjListPtr->at(curr)) {
    if (neighbor == start) result->push_back(to_string(neighbor));
    if (visited->count(neighbor)) continue;
    result->push_back(to_string(neighbor));
    extractWithStartGivenDFS(neighbor, start, result, visited);
  }
}

shared_ptr<vector<string>> AffectsTRelationExtractor::extractWithEndGiven(
    int end) {
  shared_ptr<vector<string>> result = make_shared<vector<string>>();
  extractWithEndGivenDFS(end, end, result, make_shared<unordered_set<int>>());
  return result;
}

void AffectsTRelationExtractor::initAffectsAdjListReversed() {
  affectsAdjListReversedPtr =
      make_shared<unordered_map<int, unordered_set<int>>>();
  for (int i = 0; i < programSize; i++) {
    affectsAdjListReversedPtr->insert({i + 1, unordered_set<int>()});
  }
  for (auto &relation : *affectsTable) {
    affectsAdjListReversedPtr->at(stoi(relation[1])).insert(stoi(relation[0]));
  }
}

void AffectsTRelationExtractor::extractWithEndGivenDFS(
    int curr, int end, shared_ptr<vector<string>> result,
    shared_ptr<unordered_set<int>> visited) {
  visited->insert(curr);
  for (auto &neighbor : affectsAdjListReversedPtr->at(curr)) {
    if (neighbor == end) result->push_back(to_string(neighbor));
    if (visited->count(neighbor)) continue;
    result->push_back(to_string(neighbor));
    extractWithEndGivenDFS(neighbor, end, result, visited);
  }
}
}  // namespace DE