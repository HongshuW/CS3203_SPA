//
// Created by Aaron on 1/10/22.
//

#ifndef SPA_NEXTEXTRACTOR_H
#define SPA_NEXTEXTRACTOR_H

#include "DesignExtractor.h"
#include "CFG/CFG.h"

class NextExtractor {
public:
    static shared_ptr<list<vector<string>>> extractNext(shared_ptr<ProgramNode> programNode);
	static vector<string> extractNextStar(shared_ptr<ProgramNode> programNode, StmtNoArgs args);
    static list<vector<string>> extractAllNextStarInProgram(shared_ptr<ProgramNode> programNode);
private:
    static vector<string> extractNextStarWithEndOnly(shared_ptr<ProgramNode> programNode, StmtNoArgs args);
    static vector<string> extractNextStarWithStartOnly(shared_ptr<ProgramNode> programNode, StmtNoArgs args);
    static vector<string> extractNextStarWithStartAndEnd(shared_ptr<ProgramNode> programNode, StmtNoArgs args);
    static void extractNextStarWithStartAndEndDFSHelper(int start, int end, CFG cfg, vector<string>& ans, vector<bool>& visitedArr);
    static bool areBothArgsVaild(shared_ptr<ProgramNode> programNode, int start, int end);
};

#endif //SPA_NEXTEXTRACTOR_H
