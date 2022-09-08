//
// Created by Tan Wei En on 2/9/22.
//

#include <vector>
#include <unordered_set>

#ifndef SPA_TOKENIZER_H
#define SPA_TOKENIZER_H

#endif //SPA_TOKENIZER_H

// maybe include method to tokenise symbols

namespace SourceParser {
    class Tokenizer {
    private:
        const std::unordered_set<std::string> SYMBOL_SET = {"(", ")", ",", "{", "}", ";", "=", "+", "-", ">", "<", "!"};
        unsigned int currIdx;
        std::string source;
        std::string curr;
        char peek();
        char pop();
        void processSymbols();
        void processString();
        void processAlNum();

    public:
        std::vector<std::string> tokens;
        std::vector<std::string> tokenize();
        Tokenizer(std::string source);

    };
}
