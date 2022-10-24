//
// Created by Aaron on 1/10/22.
//

#ifndef SPA_NEXTEXTRACTOR_H
#define SPA_NEXTEXTRACTOR_H

#include "CFG/CFG.h"
#include "DesignExtractor.h"
#include "args/StmtNoArgs.h"

typedef shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> StmtNoMap;
typedef shared_ptr<unordered_map<shared_ptr<ProcedureNode>, int>> FirstLineNoToProcMap;
typedef shared_ptr<unordered_map<int, shared_ptr<ProcedureNode>>> StmtNoToProcMap;

class NextExtractor {
 public:
    explicit NextExtractor(shared_ptr<ProgramNode> programNode);
  static shared_ptr<list<vector<string>>> extractNext();
  static vector<string> extractNextStar(StmtNoArgs args);
  static list<vector<string>> extractAllNextStarInProgram();

 private:
    static inline shared_ptr<ProgramNode> programNode;
    static inline StmtNoMap stmtNumbers;
    static inline FirstLineNoToProcMap firstLineNumToProcMap;
    static inline StmtNoToProcMap stmtNoToProcMap;
  static vector<string> extractNextStarWithEndOnly(StmtNoArgs args);
  static vector<string> extractNextStarWithStartOnly(StmtNoArgs args);
  static vector<string> extractNextStarWithStartAndEnd(StmtNoArgs args);
  static void extractNextStarWithStartAndEndDFSHelper(int start, int end,
                                                      CFG cfg,
                                                      vector<string>& ans,
                                                      vector<bool>& visitedArr);
  static bool areBothArgsVaild(int start, int end);
};

#endif  // SPA_NEXTEXTRACTOR_H