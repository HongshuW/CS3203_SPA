//
// Created by Xingchen Lin on 26/10/22.
//

#ifndef SPA_DESIGNEXTRACTORUTILS_H
#define SPA_DESIGNEXTRACTORUTILS_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <list>
#include "AST/CallNode.h"
#include "AST/ProgramNode.h"
#include "AST/StmtNode.h"
#include "AST/utils/ASTUtils.h"

using namespace std;

namespace DE {

		class DesignExtractorUtils {
		public:
			DesignExtractorUtils();
			  //! For calls
				unordered_map<string, vector<shared_ptr<CallNode>>>
				extractCallNodesFromProcedures(const shared_ptr<ProgramNode>& rootPtr);

				void extractVariablesFromCallNodesInProceduresToList(
								const shared_ptr<CallNode>& callNode,
								unordered_map<string, unordered_set<string>> mappedProceduresToVar,
								unordered_map<string, vector<shared_ptr<CallNode>>>
				mappedCallNodesToProcedures,
				unordered_set<string>& uniqueVarList);

				vector<shared_ptr<StmtNode>> extractIfAndWhileNodesFromProcedures(
								const shared_ptr<ProgramNode>& rootPtr);

				void extractCallStmtRelationshipsToOutput(
								int stmtNo, const shared_ptr<CallNode>& callNode,
								unordered_map<string, unordered_set<string>> mappedProceduresToVars,
								unordered_map<string, vector<shared_ptr<CallNode>>>
								mappedCallNodesToProcedures,
								const shared_ptr<list<vector<string>>>& output);
				void extractCallStmtRelationshipsWithIfAndWhileToOutput(
								const shared_ptr<ProgramNode>& rootPtr,
								unordered_map<string, unordered_set<string>> mappedProceduresToVars,
								unordered_map<string, unordered_set<string>> mappedIfAndWhileToVars,
								const shared_ptr<list<vector<string>>>& output);
		};

} // DE

#endif //SPA_DESIGNEXTRACTORUTILS_H
