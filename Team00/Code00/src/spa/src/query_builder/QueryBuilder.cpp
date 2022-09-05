//
// Created by Xingchen Lin on 27/8/22.
//

#include "query_builder/QueryBuilder.h"
#include "query_builder/QueryTokenizer.h"
#include "query_builder/QueryValidator.h"
#include "QueryParser.h"
#include <string>

using namespace QB;

shared_ptr<Query> QueryBuilder::buildPQLQuery(std::string& queryStr) {
    QueryTokenizer tokenizer = QueryTokenizer(queryStr);
    std::vector<std::string> tokens = tokenizer.tokenize();
    QueryParser parser = QueryParser(tokens);
    shared_ptr<Query> queryPtr = parser.parse();
    QueryValidator validator = QueryValidator(queryPtr);
    validator.validateQuery();
    return queryPtr;
}