//
// Created by Xingchen Lin on 27/10/22.
//

#ifndef SPA_NEXTRELATIONEXTRACTOR_H
#define SPA_NEXTRELATIONEXTRACTOR_H

#include "design_extractor/AbstractDesignExtractor.h"
#include "CFG/CFG.h"
#include "design_extractor/args/StmtNoArgs.h"
#include "design_extractor/results/RelationResult.h"

namespace DE {
		typedef shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> StmtNoMap;
		typedef shared_ptr<unordered_map<shared_ptr<ProcedureNode>, int>>
						FirstLineNoToProcMap;
		typedef shared_ptr<unordered_map<int, shared_ptr<ProcedureNode>>>
						StmtNoToProcMap;
		typedef shared_ptr<unordered_map<shared_ptr<ProcedureNode>, CFG>> ProcCFGMap;

		class NextRelationExtractor : AbstractDesignExtractor {
		private:
				FirstLineNoToProcMap firstLineNumToProcMap;
				StmtNoToProcMap stmtNoToProcMap;
				ProcCFGMap procCFGMap;

				void generateProcCFGMap();

		public:
				NextRelationExtractor(shared_ptr<DataModifier> dataModifier,
				shared_ptr<ProgramNode> programNode);
				shared_ptr<ExtractorResult> extract() override;
				void save(shared_ptr<ExtractorResult> result) override;
		};

} // DE

#endif //SPA_NEXTRELATIONEXTRACTOR_H
