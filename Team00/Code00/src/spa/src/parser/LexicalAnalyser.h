//
// Created by Tan Wei En on 2/9/22.
//

#include <string>

#ifndef SPA_LEXICALANALYSER_H
#define SPA_LEXICALANALYSER_H

#endif //SPA_LEXICALANALYSER_H

namespace SourceParser {
    class LexicalAnalyser {
    public:
        bool isValidName(std::string str);
        bool isValidInteger(std::string str);
    };
}
