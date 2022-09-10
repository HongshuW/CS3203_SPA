//
// Created by Tan Wei En on 2/9/22.
//

#include <vector>
#include <unordered_set>

#ifndef SPA_TOKENIZER_H
#define SPA_TOKENIZER_H

#endif //SPA_TOKENIZER_H

// maybe include method to tokenise symbols

using namespace std;

namespace SourceParser {
    class Tokenizer {
    private:
        const unordered_set<string> SYMBOL_SET = {"(", ")", ",", "{", "}", ";", "=",
                                                            "+", "-", ">", "<", "!", "*", "/", "%", "|", "&"};
        unsigned int currIdx;
        string source;
        string curr;
        char peek();
        char pop();
        void processSymbols();
        void processString();
        void processAlNum();
        bool match(char s);

    public:
        std::vector<std::string> tokens;
        std::vector<std::string> tokenize();
        Tokenizer(std::string source);

    };
}
