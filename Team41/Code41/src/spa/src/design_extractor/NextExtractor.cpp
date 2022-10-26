//
// Created by Aaron on 1/10/22.
//

#include "NextExtractor.h"

#include <queue>

#include "../AST/utils/ASTUtils.h"

NextExtractor::NextExtractor(shared_ptr<ProgramNode> programNode) {
  NextExtractor::programNode = programNode;
  NextExtractor::stmtNumbers = ASTUtils::getNodePtrToLineNumMap(programNode);
  NextExtractor::firstLineNumToProcMap =
      ASTUtils::getFirstLineNumToProcMap(programNode);
  NextExtractor::stmtNoToProcMap = ASTUtils::getLineNumToProcMap(programNode);
  NextExtractor::generateProcCFGMap();
}

void NextExtractor::generateProcCFGMap() {
  NextExtractor::procCFGMap =
      make_shared<unordered_map<shared_ptr<ProcedureNode>, CFG>>();
  vector<shared_ptr<ProcedureNode>> procedureList = programNode->procedureList;
  for (shared_ptr<ProcedureNode> procedure : procedureList) {
    CFG cfg = CFG(*procedure, stmtNumbers);
    NextExtractor::procCFGMap->insert({procedure, cfg});
  }
}

shared_ptr<list<vector<string>>> NextExtractor::extractNext() {
  vector<shared_ptr<ProcedureNode>> procedureList = programNode->procedureList;
  list<vector<string>> output;
  for (auto procedure : procedureList) {
    int startNum = firstLineNumToProcMap->at(procedure);
    CFG cfg = procCFGMap->at(procedure);
    int cfgSize = cfg.cfg->size() + startNum;
    for (int i = startNum; i < cfgSize; i++) {
      unordered_set<int> children = cfg.cfg->find(i)->second;
      for (int c : children) {
        vector<string> nextEntry;
        nextEntry.push_back(to_string(i));
        nextEntry.push_back(to_string(c));
        output.push_back(nextEntry);
      }
    }
  }

  return make_shared<list<vector<string>>>(output);
}

bool NextExtractor::checkNoWildcardBFS(int start, int end, CFG cfg) {
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

shared_ptr<vector<string>> NextExtractor::extractNoWildcard(StmtNoArgs args) {
  int start = args.getStartStmtNo();
  int end = args.getEndStmtNo();
  shared_ptr<vector<string>> output =
      make_shared<vector<string>>(vector<string>());
  if (!NextExtractor::areBothArgsValid(start, end)) {
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

shared_ptr<vector<string>> NextExtractor::extractOneWildcardDFS(
    CFG cfg, int startingPoint, bool isStartGiven) {
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

shared_ptr<vector<string>> NextExtractor::extractOneWildcard(
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

bool NextExtractor::areBothArgsValid(int start, int end) {
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

shared_ptr<vector<string>> NextExtractor::extractNextStar(StmtNoArgs args) {
  if (args.startAndEndExists()) {
    return extractNoWildcard(args);
  }

  if (args.startExistsOnly()) {
    return extractOneWildcard(args, true);
  }

  if (args.endExistsOnly()) {
    return extractOneWildcard(args, false);
  }

  return make_shared<vector<string>>(vector<string>());
}

list<vector<string>> NextExtractor::extractAllNextStarInProgram() {
  vector<shared_ptr<ProcedureNode>> procedureList = programNode->procedureList;
  list<vector<string>> output;
  for (auto procedure : procedureList) {
    int startNum = firstLineNumToProcMap->at(procedure);
    CFG cfg = procCFGMap->at(procedure);
    int cfgSize = cfg.cfg->size() + startNum;
    for (int i = startNum; i < cfgSize; i++) {
      StmtNoArgs args = StmtNoArgs();
      args.setStartStmtNo(i);
      vector<string> nextTStmts =
          *NextExtractor::extractOneWildcard(args, true);
      for (string nextTStmt : nextTStmts) {
        output.push_back(vector<string>{to_string(i), nextTStmt});
      }
    }
  }
  return output;
}
