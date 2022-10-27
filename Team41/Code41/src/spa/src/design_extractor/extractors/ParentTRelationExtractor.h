//
// Created by Xingchen Lin on 27/10/22.
//

#ifndef SPA_PARENTTRELATIONEXTRACTOR_H
#define SPA_PARENTTRELATIONEXTRACTOR_H

#include <map>

#include "design_extractor/AbstractDesignExtractor.h"
#include "design_extractor/results/RelationResult.h"
#include "design_extractor/utils/ParentExtractorUtils.h"

namespace DE {

class ParentTRelationExtractor : public AbstractDesignExtractor,
                                 public ParentExtractorUtils {
 private:
  void extractParentTDFS(
      const shared_ptr<map<int, vector<int>>>& parentRelations, int key,
      vector<string> ancestors, const shared_ptr<list<vector<string>>>& output);

 public:
  ParentTRelationExtractor(shared_ptr<DataModifier> dataModifier,
                           shared_ptr<ProgramNode> programNode);
  shared_ptr<ExtractorResult> extract() override;
  void save(shared_ptr<ExtractorResult> result) override;
};

}  // namespace DE

#endif  // SPA_PARENTTRELATIONEXTRACTOR_H
