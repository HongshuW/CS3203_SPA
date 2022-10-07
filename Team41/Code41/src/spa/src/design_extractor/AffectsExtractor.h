//
// Created by Aaron on 1/10/22.
//

#ifndef SPA_AFFECTSEXTRACTOR_H
#define SPA_AFFECTSEXTRACTOR_H

#include "DesignExtractor.h"
#include "args/StmtNoArgs.h"
#include "CFG/CFG.h"

class AffectsExtractor {
public:
	static vector<string> extractAffects(shared_ptr<ProgramNode> programNode, StmtNoArgs args);
	static vector<string> extractAffectsStar(shared_ptr<ProgramNode> programNode, StmtNoArgs args);
private:
	static vector<string> extractAffectsWithStartAndEnd(shared_ptr<ProgramNode> programNode, StmtNoArgs args);
	static vector<string> extractAffectsWithEndOnly(shared_ptr<ProgramNode> programNode, StmtNoArgs args);
	static vector<string> extractAffectsWithStartOnly(shared_ptr<ProgramNode> programNode, StmtNoArgs args);
	static void extractAffectsWithStartAndEndDFSHelper(int start, int end,
		CFG cfg, list<vector<string>>& validPathsList, 
		vector<string>& validPaths, vector<bool>& visitedArr);

	static vector<string> extractAffectsStarWithStartAndEnd(shared_ptr<ProgramNode> programNode, StmtNoArgs args);
	static vector<string> extractAffectsStarWithStartOnly(shared_ptr<ProgramNode> programNode, StmtNoArgs args);
	static vector<string> extractAffectsStarWithEndOnly(shared_ptr<ProgramNode> programNode, StmtNoArgs args);
    static vector<string> getIntersectionBetweenAffects(vector<string> affectsWithStartOnly,
                                                        vector<string> affectsWithEndOnly,
                                                        StmtNoArgs args);

	static unordered_map<string, shared_ptr<StmtNode>> getStmtNoToAssignReadAndCallNodesMap(shared_ptr<ProgramNode> programNode);
	static unordered_set<string> getAllStmtNoOfAssignNodes(shared_ptr<ProgramNode> programNode);
	static bool isVarModified(string modifiedVar, shared_ptr<ProgramNode> programNode, unordered_set<string> filteredStmtNoList);
	static bool areBothArgsVaild(shared_ptr<ProgramNode> programNode, int start, int end);
    static bool areBothArgsAssign(unordered_map<string, shared_ptr<StmtNode>> stmtNoToAssignReadAndCallMap,
                                  int start, int end);
};

#endif //SPA_AFFECTSEXTRACTOR_H
