//
// Created by Xingchen Lin on 26/10/22.
//

#ifndef SPA_CALLSRELATIONEXTRACTOR_H
#define SPA_CALLSRELATIONEXTRACTOR_H

#include "AST/CallNode.h"
#include "design_extractor/AbstractDesignExtractor.h"
#include "design_extractor/extractors/EntitiesExtractor.h"
#include "AST/utils/ASTUtils.h"

namespace DE {

		class CallsRelationExtractor : public AbstractDesignExtractor {
		private:
				shared_ptr<list<vector<string>>> extractCalls(
								shared_ptr<ProgramNode> programNode);
		public:
				CallsRelationExtractor(shared_ptr<DataModifier> dataModifier,
				shared_ptr<ProgramNode> programNode);
				void extract() override;

		protected:
				void save() override;
		};

} // DE

#endif //SPA_CALLSRELATIONEXTRACTOR_H
