//
// Created by Nafour on 27/10/22.
//

#ifndef SPA_MODIFIESRELATIONEXTRACTOR_H
#define SPA_MODIFIESRELATIONEXTRACTOR_H
#include "design_extractor/AbstractDesignExtractor.h"
#include "design_extractor/utils/DesignExtractorUtils.h"

namespace DE {

class ModifiesRelationExtractor : public AbstractDesignExtractor {
 protected:
  ModifiesRelationExtractor(shared_ptr<DataModifier> dataModifier,
                            shared_ptr<ProgramNode> programNode);
  void initProceduresToModifiedVarsMap();
  StrToSetMap proceduresToModifiedVarsMap;
};

}  // namespace DE

#endif  // SPA_MODIFIESRELATIONEXTRACTOR_H
