//
// Created by Aaron on 1/10/22.
//

#ifndef SPA_AFFECTSEXTRACTOR_H
#define SPA_AFFECTSEXTRACTOR_H

#include <design_extractor/mappers/AffectsMapper.h>
#include <design_extractor/mappers/CFGMapper.h>

#include "CFG/CFG.h"
#include "DesignExtractor.h"
#include "args/StmtNoArgs.h"

class AffectsExtractor {
 public:
  static vector<string> extractAffects(shared_ptr<ProgramNode> programNode,
                                       StmtNoArgs args);
  static vector<string> extractAffectsStar(shared_ptr<ProgramNode> programNode,
                                           StmtNoArgs args);

  static list<vector<string>> extractAllAffectsInProgram(
      shared_ptr<ProgramNode> programNode);
  static list<vector<string>> extractAllAffectsStarInProgram(
      shared_ptr<ProgramNode> programNode);

 private:
  static vector<string> extractAffectsWithStartAndEnd(
      shared_ptr<ProgramNode> programNode,
      shared_ptr<AffectsMapper>& affectsMap, shared_ptr<CFGMapper>& cfgMap,
      StmtNoArgs args);

  static vector<string> extractAffectsWithEndOnly(
      shared_ptr<ProgramNode> programNode,
      shared_ptr<AffectsMapper>& affectsMap, shared_ptr<CFGMapper>& cfgMap,
      StmtNoArgs args);

  static vector<string> extractAffectsWithStartOnly(
      shared_ptr<ProgramNode> programNode,
      shared_ptr<AffectsMapper>& affectsMap, shared_ptr<CFGMapper>& cfgMap,
      StmtNoArgs args);

  static vector<string> extractAffectsStarWithStartAndEnd(
      shared_ptr<ProgramNode> programNode,
      shared_ptr<AffectsMapper>& affectsMap, shared_ptr<CFGMapper>& cfgMap,
      StmtNoArgs args);

  static vector<string> extractAffectsStarWithStartOnly(
      shared_ptr<ProgramNode> programNode,
      shared_ptr<AffectsMapper>& affectsMap, shared_ptr<CFGMapper>& cfgMap,
      StmtNoArgs args);

  static vector<string> extractAffectsStarWithEndOnly(
      shared_ptr<ProgramNode> programNode,
      shared_ptr<AffectsMapper>& affectsMap, shared_ptr<CFGMapper>& cfgMap,
      StmtNoArgs args);

  static unordered_map<string, shared_ptr<StmtNode>>
  getStmtNoToAssignReadAndCallNodesMap(shared_ptr<ProgramNode> programNode);
  static unordered_set<string> getAllStmtNoOfAssignNodes(
      shared_ptr<ProgramNode> programNode);
  static bool isVarModified(string modifiedVar,
                            shared_ptr<AffectsMapper>& affectsMap,
                            unordered_set<string> filteredStmtNoList);
  static bool areArgsVaild(shared_ptr<AffectsMapper>& affectsMap, int start,
                           int end);
  static void getValidPathsFromCFG(int start, int end, const CFG& cfg,
                                   list<vector<string>>& validPathsList,
                                   vector<string>& validPaths,
                                   vector<bool>& visitedArr);
  static bool hasIntersectionWithinIntermediateRes(
      vector<string> affectsWithStart, vector<string> affectsWithEnd,
      shared_ptr<ProgramNode> programNode,
      shared_ptr<AffectsMapper>& affectsMap, shared_ptr<CFGMapper>& cfgMap);
};

#endif  // SPA_AFFECTSEXTRACTOR_H