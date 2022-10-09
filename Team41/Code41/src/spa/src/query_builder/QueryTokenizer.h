//
// Created by Xingchen Lin on 27/8/22.
//

#ifndef SPA_QUERYTOKENIZER_H
#define SPA_QUERYTOKENIZER_H

#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include "constants/QueryTokenizerConstants.h"
using namespace std;

namespace QB {
    class QueryTokenizer {
    private:
        vector<string> tokens;
        const unordered_set<string> SYMBOL_SET = {
                QueryTokenizerConstants::LEFT_BRACKET,
                QueryTokenizerConstants::RIGHT_BRACKET,
                QueryTokenizerConstants::COMMA,
                QueryTokenizerConstants::UNDERSCORE,
                QueryTokenizerConstants::SEMICOLON,
                QueryTokenizerConstants::LESS_THAN,
                QueryTokenizerConstants::GREATER_THAN,
                QueryTokenizerConstants::DOT,
                QueryTokenizerConstants::EQUAL
        };

        const unordered_set<string> CLAUSE_SET_WITH_T = {
                QueryTokenizerConstants::FOLLOWS,
                QueryTokenizerConstants::PARENT,
                QueryTokenizerConstants::CALLS,
                QueryTokenizerConstants::NEXT,
                QueryTokenizerConstants::AFFECTS
        };

        int currIdx;
        string query;
        string curr;
        char peek();
        char pop();
        void processIdent();
        void processDigit();
        void processString();
        bool isWithinBound();
    public:
        explicit QueryTokenizer(string query);
        vector<string> tokenize();
    };
}

#endif //SPA_QUERYTOKENIZER_H
