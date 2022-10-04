//
// Created by Xingchen Lin on 15/9/22.
//

#include "SPAManager.h"

SPAManager::SPAManager(shared_ptr<PKBStorage> pkbStorage) : pkbStorage(pkbStorage) {}

void SPAManager::parse(string &filename) {
    try {
        ASTBuilder astBuilder = ASTBuilder();
        shared_ptr<ProgramNode> programNode = astBuilder.buildAST(filename);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        designExtractor.run();
    } catch (const SPASTException& e) {
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
        auto dataRetriever = make_shared<DataRetriever>(
                DataRetriever(pkbStorage));
        auto ans = QueryEvaluator(dataRetriever).evaluate(queryObj);
        for (const auto& element: ans) {
            results.push_back(element);
        }
    } catch (const PQLTokenizeException& e) {
        string errorMessage = "SyntaxError";
        results.push_back(errorMessage);
    } catch (const PQLParseException& e) {
        string errorMessage = "SyntaxError";
        results.push_back(errorMessage);
    } catch (const PQLValidationException& e) {
        string errorMessage = "SemanticError";
        results.push_back(errorMessage);
    } catch (std::exception& e) {
        string errorMessage = e.what();
        results.push_back(errorMessage);
    }
    return results;
}