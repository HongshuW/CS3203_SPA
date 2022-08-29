//
// Created by Xingchen Lin on 27/8/22.
//

#ifndef SPA_QUERYTOKENIZER_H
#define SPA_QUERYTOKENIZER_H

#pragma once

#include <string>
#include <vector>
#include <unordered_set>

namespace QB {
    class QueryTokenizer {
    private:
        const std::unordered_set<std::string> SYMBOL_SET = {"(", ")", ",", "_", ";"};
        const std::unordered_set<std::string>  CLAUSE_SET_WITH_T = {"Follows", "Parent"};
        unsigned int currIdx;
        std::string query;
        std::string curr;
        char peek();
        char pop();
        void processIdent();
        void processDigit();
        void processString();
    public:
        std::vector<std::string> tokens;
        QueryTokenizer(std::string query);
        std::vector<std::string> tokenize();
    };
}

#endif //SPA_QUERYTOKENIZER_H
