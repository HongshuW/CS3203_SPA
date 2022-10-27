//
// Created by Xingchen Lin on 27/10/22.
//

#include "NextRelationExtractor.h"

#include <utility>

#include "../../AST/utils/ASTUtils.h"

namespace DE {
		NextRelationExtractor::NextRelationExtractor(shared_ptr<DataModifier> dataModifier,
																				 shared_ptr<ProgramNode> programNode)
						: AbstractDesignExtractor(std::move(dataModifier), std::move(programNode)) {
			generateProcCFGMap();
		}

		shared_ptr<ExtractorResult> NextRelationExtractor::extract() {
			vector<shared_ptr<ProcedureNode>> procedureList = programNode->procedureList;
			shared_ptr<list<vector<string>>> output = make_shared<list<vector<string>>>();
			firstLineNumToProcMap = ASTUtils::getFirstLineNumToProcMap(programNode);
			for (const auto& procedure : procedureList) {
				int startNum = firstLineNumToProcMap->at(procedure);
				CFG cfg = procCFGMap->at(procedure);
				int cfgSize = cfg.cfg->size() + startNum;
				for (int i = startNum; i < cfgSize; i++) {
					unordered_set<int> children = cfg.cfg->find(i)->second;
					for (int c : children) {
						vector<string> nextEntry;
						nextEntry.push_back(to_string(i));
						nextEntry.push_back(to_string(c));
						output->push_back(nextEntry);
					}
				}
			}

			return make_shared<RelationResult>(output);
		}

		void NextRelationExtractor::save(shared_ptr<ExtractorResult> result) {
			shared_ptr<RelationResult> nextResult =
							static_pointer_cast<RelationResult>(result);
			for (const auto& entry : *nextResult->getResult()) {
				dataModifier->saveNext(entry);
			}
		}

		void NextRelationExtractor::generateProcCFGMap() {
			procCFGMap = make_shared<unordered_map<shared_ptr<ProcedureNode>, CFG>>();
			vector<shared_ptr<ProcedureNode>> procedureList = programNode->procedureList;
			for (const shared_ptr<ProcedureNode>& procedure : procedureList) {
				CFG cfg = CFG(*procedure, stmtNumbers);
				procCFGMap->insert({procedure, cfg});
			}
		}

} // DE