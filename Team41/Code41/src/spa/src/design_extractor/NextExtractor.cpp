//
// Created by Aaron on 1/10/22.
//

#include <unordered_set>
#include "NextExtractor.h"
#include "../AST/utils/ASTUtils.h"
#include "../CFG/CFG.h"

vector<string> NextExtractor::extractNext(shared_ptr<ProgramNode> programNode, 
	shared_ptr<ProcedureNode> procedureNode, NextStmtNoArgs args) {
	vector<string> ans;
	/*shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
		ASTUtils::getNodePtrToLineNumMap(programNode);
	CFG cfg = CFG(*procedureNode, stmtNumbers);
	unordered_set<int> children = cfg.cfg->find(n1)->second;
	if (!children.empty()) {
		for (int stmtNo : children) {
			if (stmtNo == n2) {
				ans.push_back(to_string(n1));
				ans.push_back(to_string(n2));
			}
		}
	}*/

	return ans;
}

shared_ptr<list<vector<string>>> NextExtractor::extractNextStar(shared_ptr<ProgramNode> programNode, 
	shared_ptr<ProcedureNode> procedureNode, int n1, int n2) {
	return shared_ptr<list<vector<string>>>();
}
