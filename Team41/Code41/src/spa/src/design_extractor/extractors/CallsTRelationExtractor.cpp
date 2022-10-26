//
// Created by Xingchen Lin on 26/10/22.
//

#include "CallsTRelationExtractor.h"

#include <utility>

namespace DE {
		CallsTRelationExtractor::CallsTRelationExtractor(shared_ptr<DataModifier> dataModifier,
																										 shared_ptr<ProgramNode> programNode) : AbstractDesignExtractor(
						std::move(dataModifier), std::move(programNode)) {

		}

		void CallsTRelationExtractor::extract() {

		}

		void CallsTRelationExtractor::save() {
			shared_ptr<list<vector<string>>> callsStarList = extractCallsStar(programNode);

			for (const auto& entry : *callsStarList) {
				dataModifier->saveCallsT(entry);
			}
		}

		shared_ptr<list<vector<string>>> CallsTRelationExtractor::extractCallsStar(const shared_ptr<ProgramNode>& programNode) {
			list<vector<string>> output;
			DesignExtractorUtils designExtractorUtils = DesignExtractorUtils();
			auto mappedCallNodesToProcedures =
							designExtractorUtils.extractCallNodesFromProcedures(programNode);
			shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
							ASTUtils::getNodePtrToLineNumMap(programNode);
			for (auto& it : mappedCallNodesToProcedures) {
				string name = it.first;
				vector<shared_ptr<CallNode>> listOfCallNodes = it.second;
				unordered_set<string> reachableCallNodeNames;
				for (const auto& callNode : listOfCallNodes) {
					queue<shared_ptr<CallNode>> queue;
					queue.push(callNode);
					while (!queue.empty()) {
						auto callNodeEntry = queue.front();
						queue.pop();
						reachableCallNodeNames.insert(callNodeEntry->procedureName);
						if (mappedCallNodesToProcedures.count(callNodeEntry->procedureName) !=
								0) {
							auto otherCallNodes =
											mappedCallNodesToProcedures.at(callNodeEntry->procedureName);
							for (const auto& n : otherCallNodes) {
								queue.push(n);
							}
						}
					}
				}
				for (const auto& nodeNames : reachableCallNodeNames) {
					vector<string> callsEntry;
					callsEntry.push_back(name);
					callsEntry.push_back(nodeNames);
					output.push_back(callsEntry);
				}
			}
			return make_shared<list<vector<string>>>(output);
		}
} // DE