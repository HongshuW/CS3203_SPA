//
// Created by Nafour on 27/10/22.
//

#ifndef SPA_MODIFIESRELATIONEXTRACTOR_H
#define SPA_MODIFIESRELATIONEXTRACTOR_H
#include "design_extractor/AbstractDesignExtractor.h"
#include "design_extractor/extractors/interfaces/CallNodeExtractionInterface.h"

namespace DE {

class ModifiesRelationExtractor : public AbstractDesignExtractor,
                                  public CallNodeExtractionInterface {
 protected:
  ModifiesRelationExtractor(shared_ptr<DataModifier> dataModifier,
                            shared_ptr<ProgramNode> programNode);
  void initProceduresToModifiedVarsMap();
  StrToSetMap proceduresToModifiedVarsMap;
};

}  // namespace DE

#endif  // SPA_MODIFIESRELATIONEXTRACTOR_H
