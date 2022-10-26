//
// Created by Xingchen Lin on 26/10/22.
//

#include "CallsRelationExtractor.h"

namespace DE {
		CallsRelationExtractor::CallsRelationExtractor(shared_ptr<DataModifier> dataModifier,
																									 shared_ptr<ProgramNode> programNode) : AbstractDesignExtractor(
						dataModifier, programNode) {

		}

		void CallsRelationExtractor::extract() {
			shared_ptr<list<vector<string>>> extractedRelations = extractCalls(programNode);

		}

		void CallsRelationExtractor::save() {
			shared_ptr<list<vector<string>>> callsList = extractCalls(programNode);
			for (auto entry : *callsList) {
				dataModifier->saveCalls(entry);
			}
		}

		shared_ptr<list<vector<string>>> CallsRelationExtractor::extractCalls(shared_ptr<ProgramNode> programNode) {
			list<vector<string>> output;
			DesignExtractorUtils designExtractorUtils = DesignExtractorUtils();
			auto mappedCallNodesToProcedures =
							designExtractorUtils.extractCallNodesFromProcedures(programNode);
			shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
							ASTUtils::getNodePtrToLineNumMap(programNode);
			for (auto& it : mappedCallNodesToProcedures) {
				string name = it.first;
				vector<shared_ptr<CallNode>> listOfCallNodes = it.second;
				for (auto callNode : listOfCallNodes) {
					vector<string> callsEntry;
					string callNodeName = callNode->procedureName;
					callsEntry.push_back(name);
					callsEntry.push_back(callNodeName);
					callsEntry.push_back(to_string(stmtNumbers->at(callNode)));
					output.push_back(callsEntry);
				}
			}
			return make_shared<list<vector<string>>>(output);
		}
} // DE