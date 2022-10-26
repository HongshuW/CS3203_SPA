//
// Created by Xingchen Lin on 26/10/22.
//

#ifndef SPA_DESIGNEXTRACTORUTILS_H
#define SPA_DESIGNEXTRACTORUTILS_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include "AST/CallNode.h"
#include "AST/ProgramNode.h"
#include "AST/StmtNode.h"
#include "AST/utils/ASTUtils.h"

using namespace std;

namespace DE {

		class DesignExtractorUtils {
		public:
			DesignExtractorUtils();
				unordered_map<string, vector<shared_ptr<CallNode>>>
				extractCallNodesFromProcedures(shared_ptr<ProgramNode> rootPtr);

				void extractVariablesFromCallNodesInProceduresToList(
								shared_ptr<CallNode> callNode,
								unordered_map<string, unordered_set<string>> mappedProceduresToVar,
								unordered_map<string, vector<shared_ptr<CallNode>>>
				mappedCallNodesToProcedures,
				unordered_set<string>& uniqueVarList);

				vector<shared_ptr<StmtNode>> extractIfAndWhileNodesFromProcedures(
								shared_ptr<ProgramNode> rootPtr);
		};

} // DE

#endif //SPA_DESIGNEXTRACTORUTILS_H
