//
// Created by Aaron on 1/10/22.
//

#ifndef SPA_NEXTEXTRACTOR_H
#define SPA_NEXTEXTRACTOR_H

#include "CFG/CFG.h"
#include "DesignExtractor.h"
#include "args/StmtNoArgs.h"

class NextExtractor {
		typedef shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> StmtNoMap;
		typedef shared_ptr<unordered_map<shared_ptr<ProcedureNode>, int>>
						FirstLineNoToProcMap;
		typedef shared_ptr<unordered_map<int, shared_ptr<ProcedureNode>>>
						StmtNoToProcMap;
		typedef shared_ptr<unordered_map<shared_ptr<ProcedureNode>, CFG>> ProcCFGMap;

 public:
  explicit NextExtractor(shared_ptr<ProgramNode> programNode);
  static shared_ptr<list<vector<string>>> extractNext();
  static shared_ptr<vector<string>> extractNextStar(StmtNoArgs args);
  static list<vector<string>> extractAllNextStarInProgram();

 private:
  static inline shared_ptr<ProgramNode> programNode;
  static inline StmtNoMap stmtNumbers;
  static inline FirstLineNoToProcMap firstLineNumToProcMap;
  static inline StmtNoToProcMap stmtNoToProcMap;
  static inline ProcCFGMap procCFGMap;

  static void generateProcCFGMap();
  static shared_ptr<vector<string>> extractNoWildcard(StmtNoArgs args);
  static shared_ptr<vector<string>> extractOneWildcard(StmtNoArgs args,
                                                       bool isStartGiven);
  static bool checkNoWildcardBFS(int start, int end, CFG cfg);
  static shared_ptr<vector<string>> extractOneWildcardDFS(CFG cfg,
                                                          int startingStmt,
                                                          bool isStartGiven);
  static bool areBothArgsValid(int start, int end);
};

#endif  // SPA_NEXTEXTRACTOR_H