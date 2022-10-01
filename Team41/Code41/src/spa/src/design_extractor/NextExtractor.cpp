//
// Created by Aaron on 1/10/22.
//

#include <unordered_set>
#include "NextExtractor.h"
#include "../AST/utils/ASTUtils.h"
#include "../CFG/CFG.h"



vector<string> NextExtractor::extractNextWithStartAndEnd(shared_ptr<ProgramNode> programNode, NextStmtNoArgs args) {
	if (!args.isBothArgsVaild(programNode, args)) {
		return vector<string>();
	}
	int start = args.getStartStmtNo();
	int end = args.getEndStmtNo();

	auto ans = vector<string>();
	shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
		ASTUtils::getNodePtrToLineNumMap(programNode);
	auto stmtNoToProcMap = ASTUtils::getLineNumToProcMap(programNode);
	shared_ptr<ProcedureNode> procedureNode = stmtNoToProcMap->at(start);

	CFG cfg = CFG(*procedureNode, stmtNumbers);
	unordered_set<int> children = cfg.cfg->find(start)->second;
	if (!children.empty()) {
		for (int stmtNo : children) {
			if (stmtNo == end) {
				ans.push_back(to_string(start));
				ans.push_back(to_string(end));
			}
		}
	}

	return ans;
}

vector<string> NextExtractor::extractNext(shared_ptr<ProgramNode> programNode, NextStmtNoArgs args) {
	
	int start = args.getStartStmtNo();
	int end = args.getEndStmtNo();

	bool startAndEndExists = start > 0 && end > 0;
	bool onlyStart = start > 0 && end == 0;
	bool onlyEnd = start == 0 && end > 0; 

	if (startAndEndExists) {
		return extractNextWithStartAndEnd(programNode, args);
	}

	vector<string> ans;
	return ans;
}

shared_ptr<list<vector<string>>> NextExtractor::extractNextStar(shared_ptr<ProgramNode> programNode, 
	shared_ptr<ProcedureNode> procedureNode, int n1, int n2) {
	return shared_ptr<list<vector<string>>>();
}
