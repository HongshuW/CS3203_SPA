//
// Created by Xingchen Lin on 27/8/22.
//

#include "query_builder/QueryBuilder.h"
#include "query_builder/QueryTokenizer.h"
#include "query_builder/QueryValidator.h"
#include "QueryParser.h"
#include <string>

using namespace QB;

Query QueryBuilder::buildPQLQuery(std::string& queryStr) {
    QueryTokenizer tokenizer = QueryTokenizer(queryStr);
    std::vector<std::string> tokens = tokenizer.tokenize();
    QueryParser parser = QueryParser(tokens);
    Query query = parser.parse();
    QueryValidator validator = QueryValidator(&query);
    validator.validateQuery();
    return query;
}