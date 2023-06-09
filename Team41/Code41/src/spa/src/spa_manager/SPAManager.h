//
// Created by Xingchen Lin on 15/9/22.
//

#ifndef SPA_SPAMANAGER_H
#define SPA_SPAMANAGER_H

#include <cstdlib>
#include <memory>

#include "cache_manager/CacheManager.h"
#include "design_extractor/DesignExtractorManager.h"
#include "design_extractor/DesignExtractorRetriever.h"
#include "parser/ASTBuilder.h"
#include "parser/ASTValidator.h"
#include "parser/SPExceptions.h"
#include "pkb/DataModifier.h"
#include "pkb/DataRetriever.h"
#include "pkb/PKBStorage.h"
#include "query_builder/QueryBuilder.h"
#include "query_builder/exceptions/Exceptions.h"
#include "query_evaluator/DataPreprocessor.h"
#include "query_evaluator/QueryEvaluator.h"

using namespace SourceParser;
using namespace DE;
using namespace QE;

class SPAManager {
  //! Entry point of the program, each program should have one SPAManager
 private:
  shared_ptr<PKBStorage> pkbStorage;
  shared_ptr<CacheManager> cacheManager;

 public:
  explicit SPAManager(shared_ptr<PKBStorage> pkbStorage);

  //! To parse source program
  void parse(string& filename);
  //! To evaluate query
  vector<string> evaluate(string& query);
};

#endif  // SPA_SPAMANAGER_H
