//
// Created by Aaron on 7/10/22.
//

#include "AffectsExtractor.h"
//
// Created by Aaron on 1/10/22.
//

#include <algorithm>
#include <queue>

#include "AST/ProcedureNode.h"
#include "AffectsExtractor.h"
#include "EntityExtractor.h"
#include "ModifiesExtractor.h"
using namespace DE;

vector<string> AffectsExtractor::extractAffects(
    shared_ptr<ProgramNode> programNode, StmtNoArgs args) {
  if (args.startAndEndExists()) {
    shared_ptr<AffectsMapper> affectsMapper =
        make_shared<AffectsMapper>(programNode);
    shared_ptr<CFGMapper> cfgMapper = make_shared<CFGMapper>(programNode);
    return AffectsExtractor::extractAffectsWithStartAndEnd(
        programNode, affectsMapper, cfgMapper, args);
  }

  if (args.startExistsOnly()) {
    shared_ptr<AffectsMapper> affectsMapper =
        make_shared<AffectsMapper>(programNode);
    shared_ptr<CFGMapper> cfgMapper = make_shared<CFGMapper>(programNode);
    return AffectsExtractor::extractAffectsWithStartOnly(
        programNode, affectsMapper, cfgMapper, args);
  }

  if (args.endExistsOnly()) {
    shared_ptr<AffectsMapper> affectsMapper =
        make_shared<AffectsMapper>(programNode);
    shared_ptr<CFGMapper> cfgMapper = make_shared<CFGMapper>(programNode);
    return AffectsExtractor::extractAffectsWithEndOnly(
        programNode, affectsMapper, cfgMapper, args);
  }

  return {};
}

vector<string> AffectsExtractor::extractAffectsStar(
    shared_ptr<ProgramNode> programNode, StmtNoArgs args) {
  if (args.startAndEndExists()) {
    shared_ptr<AffectsMapper> affectsMapper =
        make_shared<AffectsMapper>(programNode);
    shared_ptr<CFGMapper> cfgMapper = make_shared<CFGMapper>(programNode);

    return AffectsExtractor::extractAffectsStarWithStartAndEnd(
        programNode, affectsMapper, cfgMapper, args);
  }

  if (args.startExistsOnly()) {
    shared_ptr<AffectsMapper> affectsMapper =
        make_shared<AffectsMapper>(programNode);
    shared_ptr<CFGMapper> cfgMapper = make_shared<CFGMapper>(programNode);
    return AffectsExtractor::extractAffectsStarWithStartOnly(
        programNode, affectsMapper, cfgMapper, args);
  }

  if (args.endExistsOnly()) {
    shared_ptr<AffectsMapper> affectsMapper =
        make_shared<AffectsMapper>(programNode);
    shared_ptr<CFGMapper> cfgMapper = make_shared<CFGMapper>(programNode);
    return AffectsExtractor::extractAffectsStarWithEndOnly(
        programNode, affectsMapper, cfgMapper, args);
  }

  return {};
}

list<vector<string>> AffectsExtractor::extractAllAffectsInProgram(
    shared_ptr<ProgramNode> programNode) {
  list<vector<string>> output;
  unordered_set<string> allStmtNoOfAssignNodes =
      AffectsExtractor::getAllStmtNoOfAssignNodes(programNode);
  shared_ptr<AffectsMapper> affectsMap =
      make_shared<AffectsMapper>(programNode);
  shared_ptr<CFGMapper> cfgMap = make_shared<CFGMapper>(programNode);
  for (auto startStmtNo : allStmtNoOfAssignNodes) {
    for (auto endStmtNo : allStmtNoOfAssignNodes) {
      StmtNoArgs args;
      args.setStartAndEndStmtNo(stoi(startStmtNo), stoi(endStmtNo));
      vector<string> ans = AffectsExtractor::extractAffectsWithStartAndEnd(
          programNode, affectsMap, cfgMap, args);
      if (!ans.empty()) {
        output.push_back(ans);
      }
    }
  }

  return output;
}

list<vector<string>> AffectsExtractor::extractAllAffectsStarInProgram(
    shared_ptr<ProgramNode> programNode) {
  list<vector<string>> output;
  unordered_set<string> allStmtNoOfAssignNodes =
      AffectsExtractor::getAllStmtNoOfAssignNodes(programNode);
  shared_ptr<AffectsMapper> affectsMap =
      make_shared<AffectsMapper>(programNode);
  shared_ptr<CFGMapper> cfgMap = make_shared<CFGMapper>(programNode);
  for (auto startStmtNo : allStmtNoOfAssignNodes) {
    for (auto endStmtNo : allStmtNoOfAssignNodes) {
      StmtNoArgs args;
      args.setStartAndEndStmtNo(stoi(startStmtNo), stoi(endStmtNo));
      vector<string> ans = AffectsExtractor::extractAffectsStarWithStartAndEnd(
          programNode, affectsMap, cfgMap, args);
      if (!ans.empty()) {
        output.push_back(ans);
      }
    }
  }
  return output;
}

unordered_map<string, shared_ptr<StmtNode>>
AffectsExtractor::getStmtNoToAssignReadAndCallNodesMap(
    shared_ptr<ProgramNode> programNode) {
  auto map = unordered_map<string, shared_ptr<StmtNode>>();
  shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
      ASTUtils::getNodePtrToLineNumMap(programNode);
  for (auto procedureNode : programNode->procedureList) {
    queue<vector<shared_ptr<StmtNode>>> queue;
    queue.push(procedureNode->stmtList);
    while (!queue.empty()) {
      auto stmtList = queue.front();
      queue.pop();
      for (auto stmtNode : stmtList) {
        NodeType nodeType = ASTUtils::getNodeType(stmtNode);
        switch (nodeType) {
          case AST::IF_NODE: {
            shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(stmtNode);
            vector<shared_ptr<StmtNode>> ifStmtList = ifNode->ifStmtList;
            vector<shared_ptr<StmtNode>> elseStmtList = ifNode->elseStmtList;
            queue.push(ifStmtList);
            queue.push(elseStmtList);
            break;
          }
          case AST::WHILE_NODE: {
            shared_ptr<WhileNode> whileNode =
                dynamic_pointer_cast<WhileNode>(stmtNode);
            vector<shared_ptr<StmtNode>> whileStmtList = whileNode->stmtList;
            queue.push(whileStmtList);
            break;
          }

          case AST::ASSIGN_NODE: {
            shared_ptr<AssignNode> assignNode =
                dynamic_pointer_cast<AssignNode>(stmtNode);
            int stmtNo = stmtNumbers->at(assignNode);
            map.insert({to_string(stmtNo), stmtNode});
            break;
          }

          case AST::READ_NODE: {
            shared_ptr<ReadNode> readNode =
                dynamic_pointer_cast<ReadNode>(stmtNode);
            int stmtNo = stmtNumbers->at(readNode);
            map.insert({to_string(stmtNo), stmtNode});
            break;
          }
          case AST::CALL_NODE: {
            shared_ptr<CallNode> callNode =
                dynamic_pointer_cast<CallNode>(stmtNode);
            int stmtNo = stmtNumbers->at(callNode);
            map.insert({to_string(stmtNo), stmtNode});
            break;
          }
          default:
            break;
        }
      }
    }
  }

  return map;
}

unordered_set<string> AffectsExtractor::getAllStmtNoOfAssignNodes(
    shared_ptr<ProgramNode> programNode) {
  unordered_set<string> stmtNoList;
  auto stmtNoToAssignReadAndCallMap =
      AffectsExtractor::getStmtNoToAssignReadAndCallNodesMap(programNode);
  for (auto ele : stmtNoToAssignReadAndCallMap) {
    string num = ele.first;
    shared_ptr<StmtNode> stmtNode = ele.second;
    NodeType type = ASTUtils::getNodeType(stmtNode);
    if (type == AST::ASSIGN_NODE) {
      stmtNoList.insert(num);
    }
  }

  return stmtNoList;
}

vector<string> AffectsExtractor::extractAffectsWithStartAndEnd(
    shared_ptr<ProgramNode> programNode, shared_ptr<AffectsMapper>& affectsMap,
    shared_ptr<CFGMapper>& cfgMap, StmtNoArgs args) {
  int startStmtNo = args.getStartStmtNo();
  int endStmtNo = args.getEndStmtNo();

  if (!areArgsVaild(affectsMap, startStmtNo, endStmtNo)) {
    return {};
  }

  auto stmtNoToProcMap = affectsMap->getLineNumToProcMap();
  auto stmtNumbers = affectsMap->getNodePtrToLineNumMap();

  shared_ptr<ProcedureNode> procedureNode = stmtNoToProcMap->at(startStmtNo);
  unordered_map<shared_ptr<ProcedureNode>, CFG> procNodeToCFGMap =
      cfgMap->getProcNodeToCFGMap();
  CFG cfg = procNodeToCFGMap.at(procedureNode);
  unordered_set<int> children = cfg.cfg->find(startStmtNo)->second;

  int visitedArrSize = cfg.cfg->size() + startStmtNo + 1;
  vector<bool> visitedArr;
  for (int i = 0; i < visitedArrSize; i++) {
    visitedArr.push_back(false);
  }
  unordered_set<string> uniqueStmtNoList;
  list<vector<string>> validPathsList;
  vector<string> validPaths;
  getValidPathsFromCFG(startStmtNo, endStmtNo, cfg, validPathsList, validPaths,
                       visitedArr);

  // can't reach end from start, hence invalid
  if (validPathsList.empty()) {
    return {};
  }

  //generate possible new paths from while nodes
  list<vector<string>> tempValidPathsList;
  for (auto path: validPathsList) {
    auto stmtNoToWhileNodeMap = affectsMap->getStmtNoOfWhileNodesMap();
    for (int i = 0; i < path.size(); i++) {
      string stmtNode = path[i];
      // if node is while and not the last node in path
      if (stmtNoToWhileNodeMap.count(stmtNode) != 0
          && i + 1 < path.size()) {
        int whileIndex = i;
        int childIndex = -1;
        int nextNode = stoi(path[i + 1]);
        unordered_set<int> whileChildren = cfg.cfg->find(stoi(stmtNode))->second;
        for (int c: whileChildren) {
          if (c != nextNode) {
            for (int j = i + 1; j < path.size(); j++) {
              if (path[j] == to_string(c)) {
                childIndex = j;
                break;
              }
            }
          }
        }

        //found alternate path
        if (childIndex != -1) {
          vector<string> tempValidPath;
          for (int k = 0; k < whileIndex + 1; k++) {
            tempValidPath.push_back(path[k]);
          }
          for (int l = childIndex; l < path.size(); l++) {
            tempValidPath.push_back(path[l]);
          }
          tempValidPathsList.push_back(tempValidPath);
        }
      }
    }
  }

  for (auto tempValid: tempValidPathsList) {
    validPathsList.push_back(tempValid);
  }

  auto stmtNoToAssignReadAndCallMap =
      affectsMap->getStmtNoOfAssignReadAndCallNodesMap();
  // handle visited nodes from paths
  for (auto path : validPathsList) {
    unordered_set<string> filteredStmtNoList;
    for (auto var : path) {
      if (var != to_string(startStmtNo)) {
        if (stmtNoToAssignReadAndCallMap.count(var) != 0) {
          filteredStmtNoList.insert(var);
        }
      }
    }

    // all visited nodes are not assign, read or call nodes
    // hence path is vaild as variable not modified
    vector<string> ans = {};
    if (filteredStmtNoList.empty()) {
      ans.push_back(to_string(startStmtNo));
      ans.push_back(to_string(endStmtNo));
      return ans;
    }

    auto startStmtNode =
        stmtNoToAssignReadAndCallMap.at(to_string(startStmtNo));
    shared_ptr<AssignNode> startAssignNode =
        dynamic_pointer_cast<AssignNode>(startStmtNode);
    string modifiedVar = startAssignNode->variableNode->variable;

    if (!isVarModified(modifiedVar, affectsMap, filteredStmtNoList)) {
      ans.push_back(to_string(startStmtNo));
      ans.push_back(to_string(endStmtNo));
      return ans;
    }
  }
  return {};
}

bool AffectsExtractor::areArgsVaild(shared_ptr<AffectsMapper>& affectsMap,
                                    int start, int end) {
  bool isValid = false;
  auto startToEndArgsMap = affectsMap->getValidStartToEndArgsMap();
  if (startToEndArgsMap.count(start) != 0) {
    unordered_set<int> endList = startToEndArgsMap.at(start);
    if (endList.count(end) != 0) {
      isValid = true;
    }
  }

  return isValid;
}

bool AffectsExtractor::isVarModified(string modifiedVar,
                                     shared_ptr<AffectsMapper>& affectsMap,
                                     unordered_set<string> filteredStmtNoList) {
  auto modifiedVarList = affectsMap->getModifiedVarMap();
  for (auto stmtNo : filteredStmtNoList) {
    auto currModifiedVarList = modifiedVarList.at(stoi(stmtNo));
    for (auto var : currModifiedVarList) {
      if (var == modifiedVar) {
        return true;
      }
    }
  }
  return false;
}
void AffectsExtractor::getValidPathsFromCFG(
    int start, int end, const CFG& cfg, list<vector<string>>& validPathsList,
    vector<string>& validPaths, vector<bool>& visitedArr) {
  unordered_set<int> children = cfg.cfg->find(start)->second;
  validPaths.push_back(to_string(start));

  if (!children.empty()) {
    for (int stmtNo : children) {
      if (stmtNo == end) {
        validPathsList.push_back(validPaths);
      }

      else if (stmtNo > start) {
        getValidPathsFromCFG(stmtNo, end, cfg, validPathsList, validPaths,
                             visitedArr);
      }

      else if (stmtNo <= start) {
        if (!visitedArr.at(stmtNo)) {
          visitedArr.at(stmtNo) = true;
          getValidPathsFromCFG(stmtNo, end, cfg, validPathsList, validPaths,
                               visitedArr);
        }
      }
    }
  }

  validPaths.pop_back();
}

vector<string> AffectsExtractor::extractAffectsWithStartOnly(
    shared_ptr<ProgramNode> programNode, shared_ptr<AffectsMapper>& affectsMap,
    shared_ptr<CFGMapper>& cfgMap, StmtNoArgs args) {
  vector<string> ans;
  auto validStartToEndArgsMap = affectsMap->getValidStartToEndArgsMap();
  int start = args.getStartStmtNo();
  if (validStartToEndArgsMap.count(start) == 0) {
    return ans;
  }
  auto endList = validStartToEndArgsMap.at(start);
  for (auto end : endList) {
    StmtNoArgs currArgs = StmtNoArgs();
    currArgs.setStartAndEndStmtNo(start, end);
    vector<string> output = AffectsExtractor::extractAffectsWithStartAndEnd(
        programNode, affectsMap, cfgMap, currArgs);
    if (!output.empty()) {
      ans.push_back(to_string(end));
    }
  }

  return ans;
}

vector<string> AffectsExtractor::extractAffectsWithEndOnly(
    shared_ptr<ProgramNode> programNode, shared_ptr<AffectsMapper>& affectsMap,
    shared_ptr<CFGMapper>& cfgMap, StmtNoArgs args) {
  vector<string> ans;
  auto validEndToStartArgsMap = affectsMap->getValidEndToStartArgsMap();
  int end = args.getEndStmtNo();
  if (validEndToStartArgsMap.count(end) == 0) {
    return ans;
  }
  auto startList = validEndToStartArgsMap.at(end);
  for (auto start : startList) {
    StmtNoArgs currArgs = StmtNoArgs();
    currArgs.setStartAndEndStmtNo(start, end);
    vector<string> output = AffectsExtractor::extractAffectsWithStartAndEnd(
        programNode, affectsMap, cfgMap, currArgs);
    if (!output.empty()) {
      ans.push_back(to_string(start));
    }
  }

  return ans;
}

vector<string> AffectsExtractor::extractAffectsStarWithStartOnly(
    shared_ptr<ProgramNode> programNode, shared_ptr<AffectsMapper>& affectsMap,
    shared_ptr<CFGMapper>& cfgMap, StmtNoArgs args) {
  vector<string> ans;
  int start = args.getStartStmtNo();
  auto stmtNoToAssignNodeMap = affectsMap->getStmtNoOfAssignNodesMap();

  for (const auto& [end, stmtNode] : stmtNoToAssignNodeMap) {
    StmtNoArgs currArgs = StmtNoArgs();
    currArgs.setStartAndEndStmtNo(start, stoi(end));
    vector<string> output = AffectsExtractor::extractAffectsStarWithStartAndEnd(
        programNode, affectsMap, cfgMap, currArgs);
    if (!output.empty()) {
      ans.push_back(end);
    }
  }

  return ans;

  return vector<string>();
}

vector<string> AffectsExtractor::extractAffectsStarWithStartAndEnd(
    shared_ptr<ProgramNode> programNode, shared_ptr<AffectsMapper>& affectsMap,
    shared_ptr<CFGMapper>& cfgMap, StmtNoArgs args) {
  vector<string> output = AffectsExtractor::extractAffectsWithStartAndEnd(
      programNode, affectsMap, cfgMap, args);

  // If Affects(a1, a2) is valid, Affects*(a1, a2) is also valid
  if (!output.empty()) {
    return output;
  }

  // for Affects*(a1, a2)
  // if there is an intersection between Affects(a1, _) and Affects(_, a2)
  // then Affects*(a1, a2) is valid
  int start = args.getStartStmtNo();
  int end = args.getEndStmtNo();

  StmtNoArgs startArgsOnly = StmtNoArgs();
  startArgsOnly.setStartStmtNo(start);

  StmtNoArgs endArgsOnly = StmtNoArgs();
  endArgsOnly.setEndStmtNo(end);

  vector<string> affectsWithStart =
      AffectsExtractor::extractAffectsWithStartOnly(programNode, affectsMap,
                                                    cfgMap, startArgsOnly);

  vector<string> affectsWithEnd = AffectsExtractor::extractAffectsWithEndOnly(
      programNode, affectsMap, cfgMap, endArgsOnly);
  vector<string> intersection_output;

  sort(affectsWithStart.begin(), affectsWithStart.end());
  sort(affectsWithEnd.begin(), affectsWithEnd.end());

  set_intersection(affectsWithStart.begin(), affectsWithStart.end(),
                   affectsWithEnd.begin(), affectsWithEnd.end(),
                   back_inserter(intersection_output));

  if (!intersection_output.empty()) {
    vector<string> ans;
    ans.push_back(to_string(start));
    ans.push_back(to_string(end));
    return ans;
  } else {
    bool intermediateResHasIntersection =
        AffectsExtractor::hasIntersectionWithinIntermediateRes(
            affectsWithStart, affectsWithEnd, programNode, affectsMap, cfgMap);
    if (intermediateResHasIntersection) {
      vector<string> ans;
      ans.push_back(to_string(start));
      ans.push_back(to_string(end));
      return ans;
    }
  }

  return {};
}

vector<string> AffectsExtractor::extractAffectsStarWithEndOnly(
    shared_ptr<ProgramNode> programNode, shared_ptr<AffectsMapper>& affectsMap,
    shared_ptr<CFGMapper>& cfgMap, StmtNoArgs args) {
  vector<string> ans;
  int end = args.getEndStmtNo();
  auto stmtNoToAssignNodeMap = affectsMap->getStmtNoOfAssignNodesMap();

  for (const auto& [start, stmtNode] : stmtNoToAssignNodeMap) {
    StmtNoArgs currArgs = StmtNoArgs();
    currArgs.setStartAndEndStmtNo(stoi(start), end);
    vector<string> output = AffectsExtractor::extractAffectsStarWithStartAndEnd(
        programNode, affectsMap, cfgMap, currArgs);
    if (!output.empty()) {
      ans.push_back(start);
    }
  }

  return ans;
}

bool AffectsExtractor::hasIntersectionWithinIntermediateRes(
    vector<string> affectsWithStart, vector<string> affectsWithEnd,
    shared_ptr<ProgramNode> programNode, shared_ptr<AffectsMapper>& affectsMap,
    shared_ptr<CFGMapper>& cfgMap) {
  if (affectsWithStart.empty() || affectsWithEnd.empty()) {
    return false;
  }

  queue<string> intermediateResQ;
  for (auto affectsStart : affectsWithStart) {
    intermediateResQ.push(affectsStart);
  }

  unordered_set<string> visitedEntries;

  while (!intermediateResQ.empty()) {
    string currEntry = intermediateResQ.front();
    intermediateResQ.pop();
    if (visitedEntries.count(currEntry) != 0) {
      continue;
    } else {
      visitedEntries.insert(currEntry);
    }
    StmtNoArgs currStartArgsOnly = StmtNoArgs();
    currStartArgsOnly.setStartStmtNo(stoi(currEntry));
    vector<string> currIntermediateResList =
        AffectsExtractor::extractAffectsWithStartOnly(
            programNode, affectsMap, cfgMap, currStartArgsOnly);

    if (!currIntermediateResList.empty()) {
      sort(currIntermediateResList.begin(), currIntermediateResList.end());
      sort(affectsWithEnd.begin(), affectsWithEnd.end());
      vector<string> intersection_output;

      set_intersection(currIntermediateResList.begin(),
                       currIntermediateResList.end(), affectsWithEnd.begin(),
                       affectsWithEnd.end(),
                       back_inserter(intersection_output));
      if (!intersection_output.empty()) {
        return true;
      } else {
        for (auto curr : currIntermediateResList) {
          intermediateResQ.push(curr);
        }
      }
    }
  }

  return false;
}
