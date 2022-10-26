//
// Created by Xingchen Lin on 26/10/22.
//

#ifndef SPA_CALLSTRELATIONEXTRACTOR_H
#define SPA_CALLSTRELATIONEXTRACTOR_H

#include "AST/CallNode.h"
#include "design_extractor/AbstractDesignExtractor.h"
#include "design_extractor/extractors/EntitiesExtractor.h"
#include "design_extractor/utils/DesignExtractorUtils.h"
#include "AST/utils/ASTUtils.h"

namespace DE {

		class CallsTRelationExtractor : public AbstractDesignExtractor {
		private:
				shared_ptr<list<vector<string>>> extractCallsStar(
								const shared_ptr<ProgramNode>& programNode);
		public:
				CallsTRelationExtractor(shared_ptr<DataModifier> dataModifier,
															 shared_ptr<ProgramNode> programNode);
				void extract() override;

		protected:
				void save() override;
		};

} // DE

#endif //SPA_CALLSTRELATIONEXTRACTOR_H
