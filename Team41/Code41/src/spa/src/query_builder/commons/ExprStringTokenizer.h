//
// Created by Xingchen Lin on 13/9/22.
//

#ifndef SPA_EXPRSTRINGTOKENIZER_H
#define SPA_EXPRSTRINGTOKENIZER_H

#include <string>
#include <vector>

#include "query_builder/constants/QueryTokenizerConstants.h"
#include "query_builder/exceptions/Exceptions.h"
#include "utils/Utils.h"

using namespace std;

namespace QB {
class ExprStringTokenizer {
 private:
  unsigned int currIdx;
  string exprStr;
  string curr;
  char peek();
  char pop();
  void processAlNum();
  bool isNotFirstAndLastChar(int index);
  vector<string> exprTokens;

 public:
  explicit ExprStringTokenizer(string exprStr);
  vector<string> tokenize();
};

}  // namespace QB

#endif  // SPA_EXPRSTRINGTOKENIZER_H
