//
// Created by Aaron on 1/10/22.
//

#include "NextExtractor.h"

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

void NextExtractor::extractOneWildcardDFS(CFG cfg) {
  // TODO: implement O(n) algorithm
}

vector<string> NextExtractor::extractOneWildcard(StmtNoArgs args,
                                                 bool isStartGiven) {
  vector<string> result = vector<string>();
  int startingPoint =
      isStartGiven ? args.getStartStmtNo() : args.getEndStmtNo();
  bool invalidArg =
      stmtNoToProcMap->count(startingPoint) == 0 && startingPoint != 0;
  if (invalidArg) {
    return {};
  }
  shared_ptr<ProcedureNode> procedureNode = stmtNoToProcMap->at(startingPoint);
  CFG cfg = procCFGMap->at(procedureNode);
  int startNum = firstLineNumToProcMap->at(procedureNode);
  int endNum = cfg.cfg->size() + startNum;

  for (int i = startNum; i < endNum; i++) {
    StmtNoArgs testArgs;
    if (isStartGiven) {
      testArgs.setStartAndEndStmtNo(startingPoint, i);
    } else {
      testArgs.setStartAndEndStmtNo(i, startingPoint);
    }
    vector<string> output =
        NextExtractor::extractNextStarWithStartAndEnd(testArgs);
    if (!output.empty()) {
      result.push_back(to_string(i));
    }
  }

  return result;
}

vector<string> NextExtractor::extractNextStarWithStartAndEnd(StmtNoArgs args) {
  int start = args.getStartStmtNo();
  int end = args.getEndStmtNo();
  auto ans = vector<string>();
  if (!NextExtractor::areBothArgsVaild(start, end)) {
    return {};
  }

  shared_ptr<ProcedureNode> procedureNode = stmtNoToProcMap->at(start);
  CFG cfg = procCFGMap->at(procedureNode);
  int visitedArrSize = cfg.cfg->size() + start + 1;
  vector<bool> visitedArr;
  for (int i = 0; i < visitedArrSize; i++) {
    visitedArr.push_back(false);
  }

  NextExtractor::extractNextStarWithStartAndEndDFSHelper(start, end, cfg, ans,
                                                         visitedArr);
  if (!ans.empty()) {
    ans = vector<string>();
    ans.push_back(to_string(start));
    ans.push_back(to_string(end));
  }
  return ans;
}

void NextExtractor::extractNextStarWithStartAndEndDFSHelper(
    int start, int end, CFG cfg, vector<string>& ans,
    vector<bool>& visitedArr) {
  unordered_set<int> children = cfg.cfg->find(start)->second;
  if (!children.empty()) {
    for (int stmtNo : children) {
      if (stmtNo == end) {
        ans.push_back(to_string(end));
        return;
      } else if (stmtNo > start) {
        extractNextStarWithStartAndEndDFSHelper(stmtNo, end, cfg, ans,
                                                visitedArr);
      }

      else if (stmtNo <= start) {
        if (!visitedArr.at(stmtNo)) {
          visitedArr.at(stmtNo) = true;
          extractNextStarWithStartAndEndDFSHelper(stmtNo, end, cfg, ans,
                                                  visitedArr);
        }
      }
    }
  }
}

bool NextExtractor::areBothArgsVaild(int start, int end) {
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

vector<string> NextExtractor::extractNextStar(StmtNoArgs args) {
  int start = args.getStartStmtNo();
  int end = args.getEndStmtNo();
  if (args.startAndEndExists()) {
    return extractNextStarWithStartAndEnd(args);
  }

  if (args.startExistsOnly()) {
    return extractOneWildcard(args, true);
  }

  if (args.endExistsOnly()) {
    return extractOneWildcard(args, false);
  }

  vector<string> ans;
  return ans;
}

list<vector<string>> NextExtractor::extractAllNextStarInProgram() {
  vector<shared_ptr<ProcedureNode>> procedureList = programNode->procedureList;
  list<vector<string>> output;
  for (auto procedure : procedureList) {
    int startNum = firstLineNumToProcMap->at(procedure);
    CFG cfg = procCFGMap->at(procedure);
    int cfgSize = cfg.cfg->size() + startNum;
    for (int i = startNum; i < cfgSize; i++) {
      for (int j = startNum; j < cfgSize; j++) {
        StmtNoArgs args;
        args.setStartAndEndStmtNo(i, j);
        vector<string> ans =
            NextExtractor::extractNextStarWithStartAndEnd(args);
        if (!ans.empty()) {
          output.push_back(ans);
        }
      }
    }
  }

  return output;
}
