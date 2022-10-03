//
// Created by Xingchen Lin on 27/8/22.
//

#ifndef SPA_QUERYTOKENIZER_H
#define SPA_QUERYTOKENIZER_H

#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include "constants/QueryTokeniserConstants.h"
using namespace std;

namespace QB {
    class QueryTokenizer {
    private:
        vector<string> tokens;
        const unordered_set<string> SYMBOL_SET = {"(", ")", ",", "_", ";", "<", ">", ".", "=", "-"};
        const unordered_set<string>  CLAUSE_SET_WITH_T = {"Follows", "Parent", "Calls", "Next", "Affects"};
        unsigned int currIdx;
        string query;
        string curr;
        char peek();
        char pop();
        void processIdent();
        void processDigit();
        void processString();
    public:
        explicit QueryTokenizer(string query);
        vector<string> tokenize();
    };
}

#endif //SPA_QUERYTOKENIZER_H
