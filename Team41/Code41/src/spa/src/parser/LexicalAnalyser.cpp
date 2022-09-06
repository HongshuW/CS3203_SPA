//
// Created by Tan Wei En on 2/9/22.

#include "LexicalAnalyser.h"

using namespace SourceParser;

bool LexicalAnalyser::isValidName(std::string str) {
    if (!isalpha(str[0])) {
        return false;
    }
    for (size_t i = 1; i < str.length(); i++) {
        if (!isalnum(str[i])) {
            return false;
        }
    }
    return true;
}

bool LexicalAnalyser::isValidInteger(std::string str) {
    for (size_t i = 0; i < str.length(); i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}