//
// Created by Xingchen Lin on 15/9/22.
//

#include "SPAManager.h"

SPAManager::SPAManager(shared_ptr<PKBStorage> pkbStorage)
    : pkbStorage(pkbStorage) {}

void SPAManager::parse(string& filename) {
  try {
    ASTBuilder astBuilder = ASTBuilder();
    shared_ptr<ProgramNode> programNode = astBuilder.buildAST(filename);

    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    DesignExtractorManager designExtractorManager =
        DesignExtractorManager(dataModifier, programNode);
    designExtractorManager.run();

    DesignExtractorRetriever designExtractorRetriever =
        DesignExtractorRetriever(programNode);
    cacheManager = make_shared<CacheManager>(CacheManager(
        make_shared<DesignExtractorRetriever>(designExtractorRetriever)));
  } catch (const SPValidationException& e) {
    exit(EXIT_SUCCESS);
  } catch (const SPTokenizeException& e) {
    exit(EXIT_SUCCESS);
  } catch (const SPParseException& e) {
    exit(EXIT_SUCCESS);
  } catch (std::exception& e) {
    exit(EXIT_SUCCESS);
  }
}

vector<string> SPAManager::evaluate(string& query) {
  vector<string> results;
  try {
    auto queryObj = QueryBuilder().buildPQLQuery(query);
    auto dataRetriever = make_shared<DataRetriever>(DataRetriever(pkbStorage));
    dataRetriever->cacheManager = cacheManager;

    auto ans = QueryEvaluator(dataRetriever).evaluate(queryObj);
    for (const auto& element : ans) {
      results.push_back(element);
    }
  } catch (const PQLTokenizeException& e) {
    const string ERROR_MESSAGE = "SyntaxError";
    results.push_back(ERROR_MESSAGE);
  } catch (const PQLParseException& e) {
    const string ERROR_MESSAGE = "SyntaxError";
    results.push_back(ERROR_MESSAGE);
  } catch (const PQLValidationException& e) {
    const string ERROR_MESSAGE = "SemanticError";
    results.push_back(ERROR_MESSAGE);
  } catch (std::exception& e) {
    const string ERROR_MESSAGE = "SyntaxError";
    results.push_back(ERROR_MESSAGE);
  }
  return results;
}