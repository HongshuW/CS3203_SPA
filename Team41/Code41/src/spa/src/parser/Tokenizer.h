//
// Created by Tan Wei En on 2/9/22.
//

#ifndef SPA_TOKENIZER_H
#define SPA_TOKENIZER_H

#include <vector>
#include <unordered_set>
#include "constants/TokeniserConstants.h"
// maybe include method to tokenise symbols

using namespace std;

namespace SourceParser {
    class Tokenizer {
    private:
        const unordered_set<string> SYMBOL_SET = {TokeniserConstants::LEFT_BRACKET,
                                                  TokeniserConstants::RIGHT_BRACKET,
                                                  TokeniserConstants::COMMA,
                                                  TokeniserConstants::CURLY_LEFT_BRACKET,
                                                  TokeniserConstants::CURLY_RIGHT_BRACKET,
                                                  TokeniserConstants::SEMICOLON,
                                                  TokeniserConstants::EQUAL,
                                                  TokeniserConstants::PLUS,
                                                  TokeniserConstants::MINUS,
                                                  TokeniserConstants::GREATER_THAN,
                                                  TokeniserConstants::SMALLER_THAN,
                                                  TokeniserConstants::EXCLAMATION_MARK,
                                                  TokeniserConstants::STAR,
                                                  TokeniserConstants::FORWARD_SLASH,
                                                  TokeniserConstants::MODULO,
                                                  TokeniserConstants::BAR,
                                                  TokeniserConstants::SINGLE_AND};
        unsigned int currIdx;
        string source;
        string curr;
        char peek();
        char pop();
        void processSymbols();
        void processString();
        void processAlNum();
        void processDigit();
        bool match(char s);

    public:
        std::vector<std::string> tokens;
        std::vector<std::string> tokenize();
        Tokenizer(std::string source);

    };
}

#endif //SPA_TOKENIZER_H