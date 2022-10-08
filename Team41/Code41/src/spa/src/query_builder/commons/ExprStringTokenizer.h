//
// Created by Xingchen Lin on 13/9/22.
//

#ifndef SPA_EXPRSTRINGTOKENIZER_H
#define SPA_EXPRSTRINGTOKENIZER_H

#include <vector>
#include <string>
#include "utils/Utils.h"
#include "query_builder/exceptions/Exceptions.h"
#include "query_builder/constants/QueryTokenizerConstants.h"

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

} // QB

#endif //SPA_EXPRSTRINGTOKENIZER_H
