//
// Created by Aaron on 1/10/22.
//

#ifndef SPA_NEXTEXTRACTOR_H
#define SPA_NEXTEXTRACTOR_H

#include "DesignExtractor.h"
#include "args/NextStmtNoArgs.h"

class NextExtractor {
public:
	static vector<string> extractNextWithStartAndEnd(shared_ptr<ProgramNode> programNode, NextStmtNoArgs args);
	static vector<string> extractNext(shared_ptr<ProgramNode> programNode, NextStmtNoArgs args);
	static shared_ptr<list<vector<string>>> extractNextStar(shared_ptr<ProgramNode> programNode, 
		shared_ptr<ProcedureNode> procedureNode, int n1, int n2);

};

#endif //SPA_NEXTEXTRACTOR_H
