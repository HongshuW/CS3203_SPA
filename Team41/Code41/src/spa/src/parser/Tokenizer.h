//
// Created by Tan Wei En on 2/9/22.
//

#ifndef SPA_TOKENIZER_H
#define SPA_TOKENIZER_H

#include <vector>
#include <unordered_set>
#include "constants/TokenizerConstants.h"
#include "utils/Utils.h"

using namespace std;

namespace SourceParser {
    class Tokenizer {
    private:
        const unordered_set<string> SYMBOL_SET = {TokenizerConstants::LEFT_BRACKET,
                                                  TokenizerConstants::RIGHT_BRACKET,
                                                  TokenizerConstants::COMMA,
                                                  TokenizerConstants::CURLY_LEFT_BRACKET,
                                                  TokenizerConstants::CURLY_RIGHT_BRACKET,
                                                  TokenizerConstants::SEMICOLON,
                                                  TokenizerConstants::EQUAL,
                                                  TokenizerConstants::PLUS,
                                                  TokenizerConstants::MINUS,
                                                  TokenizerConstants::GREATER_THAN,
                                                  TokenizerConstants::SMALLER_THAN,
                                                  TokenizerConstants::EXCLAMATION_MARK,
                                                  TokenizerConstants::STAR,
                                                  TokenizerConstants::FORWARD_SLASH,
                                                  TokenizerConstants::MODULO,
                                                  TokenizerConstants::BAR,
                                                  TokenizerConstants::SINGLE_AND};
        int currIdx;
        string source;
        string curr;
        char peek();
        char pop();
        void processSymbols();
        void processAlNum();
        void processDigit();
        bool match(char s);
        bool isWithinBound();
        vector<string> tokens;

    public:
        vector<string> tokenize();
        explicit Tokenizer(string source);
    };
}

#endif //SPA_TOKENIZER_H