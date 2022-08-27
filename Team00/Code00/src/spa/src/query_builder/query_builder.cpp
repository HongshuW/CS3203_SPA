//
// Created by Xingchen Lin on 27/8/22.
//

#include "query_builder/query_builder.h"
#include "query_builder/query_tokenizer.h"
#include <string>

using namespace QB;

std::vector<std::string> QueryBuilder::buildPQLQuery(std::string& query) {
    QueryTokenizer tokenizer = QueryTokenizer(query);
    std::vector<std::string> tokens = tokenizer.tokenize();
    // TODO: to be implemented later, change std::vector<std::string> to Query
    return tokens;
}