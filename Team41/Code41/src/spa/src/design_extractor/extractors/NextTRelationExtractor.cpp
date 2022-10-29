//
// Created by Xingchen Lin on 27/10/22.
//

#include "NextTRelationExtractor.h"

#include "NextRelationExtractor.h"

namespace DE {
NextTRelationExtractor::NextTRelationExtractor(
    shared_ptr<ProgramNode> programNode)
    : QueryTimeDesignExtractor(std::move(programNode)) {
  initialise();
}

void NextTRelationExtractor::initialise() {
  procCFGMap = generateProcCFGMap(programNode, stmtNumbers);
  stmtNoToProcMap = ASTUtils::getLineNumToProcMap(programNode);
}

shared_ptr<ExtractorResult> NextTRelationExtractor::extract(StmtNoArgs args) {
  if (args.startAndEndExists()) {
    return make_shared<QueryTimeResult>(extractNoWildcard(args));
  }

  if (args.startExistsOnly()) {
    return make_shared<QueryTimeResult>(extractOneWildcard(args, true));
  }

  if (args.endExistsOnly()) {
    return make_shared<QueryTimeResult>(extractOneWildcard(args, false));
  }

  return make_shared<QueryTimeResult>(make_shared<vector<string>>());
}

shared_ptr<ExtractorResult> NextTRelationExtractor::extractAllRelations() {
  vector<shared_ptr<ProcedureNode>> procedureList = programNode->procedureList;
  shared_ptr<list<vector<string>>> output = make_shared<list<vector<string>>>();

  FirstLineNoToProcMap firstLineNumToProcMap =
      ASTUtils::getFirstLineNumToProcMap(programNode);

  for (const auto& procedure : procedureList) {
    int startNum = firstLineNumToProcMap->at(procedure);
    CFG cfg = procCFGMap->at(procedure);
    int cfgSize = cfg.cfg->size() + startNum;
    for (int i = startNum; i < cfgSize; i++) {
      StmtNoArgs args = StmtNoArgs();
      args.setStartStmtNo(i);
      shared_ptr<vector<string>> nextTStmts = extractOneWildcard(args, true);
      for (const string& nextTStmt : *nextTStmts) {
        output->push_back(vector<string>{to_string(i), nextTStmt});
      }
    }
  }
  return make_shared<RelationResult>(output);
}

bool NextTRelationExtractor::checkNoWildcardBFS(int start, int end,
                                                const CFG& cfg) {
  // use bfs to avoid checking extremely long path without correct answer
  unordered_set<int> visited = unordered_set<int>();
  queue<int> bfsQueue = queue<int>();
  bfsQueue.push(start);
  // bfs
  while (!bfsQueue.empty()) {
    // get first element from stack and mark it as visited
    int stmt = bfsQueue.front();
    bfsQueue.pop();
    unordered_set<int> children = cfg.cfg->find(stmt)->second;
    for (int child : children) {
      if (child == end) {
        return true;
      }
      if (visited.find(child) == visited.end()) {
        // if child hasn't been visited, push it to the stack
        bfsQueue.push(child);
        visited.insert(child);
      }
    }
  }
  return false;
}

shared_ptr<vector<string>> NextTRelationExtractor::extractNoWildcard(
    StmtNoArgs args) {
  int start = args.getStartStmtNo();
  int end = args.getEndStmtNo();
  shared_ptr<vector<string>> output =
      make_shared<vector<string>>(vector<string>());
  if (!areBothArgsValid(start, end)) {
    return output;
  }
  shared_ptr<ProcedureNode> procedureNode = stmtNoToProcMap->at(start);
  CFG cfg = procCFGMap->at(procedureNode);
  if (checkNoWildcardBFS(start, end, cfg)) {
    output->push_back(to_string(start));
    output->push_back(to_string(end));
  }
  return output;
}

shared_ptr<vector<string>> NextTRelationExtractor::extractOneWildcardDFS(
    const CFG& cfg, int startingPoint, bool isStartGiven) {
  shared_ptr<vector<string>> output =
      make_shared<vector<string>>(vector<string>());
  unordered_set<int> visited = unordered_set<int>();
  stack<int> dfsStack = stack<int>();
  dfsStack.push(startingPoint);
  // dfs
  while (!dfsStack.empty()) {
    // get first element from stack and mark it as visited
    int stmt = dfsStack.top();
    dfsStack.pop();
    unordered_set<int> children = isStartGiven
                                      ? cfg.cfg->find(stmt)->second
                                      : cfg.reversedCfg->find(stmt)->second;
    for (int child : children) {
      if (visited.find(child) == visited.end()) {
        // if child hasn't been visited, push to stack and record it in output
        dfsStack.push(child);
        output->push_back(to_string(child));
        visited.insert(child);
      }
    }
  }
  return output;
}

shared_ptr<vector<string>> NextTRelationExtractor::extractOneWildcard(
    StmtNoArgs args, bool isStartGiven) {
  int startingPoint =
      isStartGiven ? args.getStartStmtNo() : args.getEndStmtNo();
  bool invalidArg =
      stmtNoToProcMap->count(startingPoint) == 0 && startingPoint != 0;
  if (invalidArg) {
    return make_shared<vector<string>>(vector<string>());
  }
  shared_ptr<ProcedureNode> procedureNode = stmtNoToProcMap->at(startingPoint);
  CFG cfg = procCFGMap->at(procedureNode);
  return extractOneWildcardDFS(cfg, startingPoint, isStartGiven);
}

bool NextTRelationExtractor::areBothArgsValid(int start, int end) {
  bool invalidStartArg = stmtNoToProcMap->count(start) == 0 && start != 0;
  bool invalidEndArg = stmtNoToProcMap->count(end) == 0 && end != 0;

  if (invalidStartArg || invalidEndArg) {
    return false;
  } else {
    if (stmtNoToProcMap->count(start) != 0 &&
        stmtNoToProcMap->count(end) != 0) {
      bool startAndEndSameProcedure =
          stmtNoToProcMap->at(start) == stmtNoToProcMap->at(end);
      if (!startAndEndSameProcedure) {
        return false;
      }
    }
  }
  return true;
}
}  // namespace DE