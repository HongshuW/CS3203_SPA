//
// Created by Xingchen Lin on 27/8/22.
//

#include "query_builder/QueryBuilder.h"

#include <string>

#include "QueryParser.h"
#include "query_builder/QueryTokenizer.h"
#include "query_builder/QueryValidator.h"

using namespace QB;

shared_ptr<Query> QueryBuilder::buildPQLQuery(string& queryStr) {
  QueryTokenizer tokenizer = QueryTokenizer(queryStr);
  vector<string> tokens = tokenizer.tokenize();
  QueryParser parser = QueryParser(tokens);
  shared_ptr<Query> queryPtr = parser.parse();
  QueryValidator validator = QueryValidator(queryPtr);
  validator.validateQuery();
  return queryPtr;
}