//
// Created by Nafour on 26/10/22.
//

#ifndef SPA_USESRELATIONEXTRACTOR_H
#define SPA_USESRELATIONEXTRACTOR_H

#include "design_extractor/AbstractDesignExtractor.h"
namespace DE {

    class UsesRelationExtractor : public AbstractDesignExtractor {
    public:
        explicit UsesRelationExtractor(shared_ptr<DataModifier> dataModifier, shared_ptr<ProgramNode> programNode);
        void extract() override;
    protected:
        void save() override;
    };

} // DE

#endif //SPA_USESRELATIONEXTRACTOR_H
