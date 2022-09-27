//
// Created by Tan Wei En on 8/9/22.
//

#ifndef SPA_SPEXCEPTIONS_H
#define SPA_SPEXCEPTIONS_H

#pragma once
#include <string>
#include <stdexcept>
#include "constants/TokeniserConstants.h"
using namespace std;

namespace SourceParser {
    /**
     * Represents an error that occurs during tokenization of
     * PQL query string.
     *
     */
    class SPTokenizeException : public runtime_error {
    public:
        explicit SPTokenizeException(const string& msg = TokeniserConstants::EMPTY_STR) : runtime_error(msg) {}
    };

    /**
    * Represents an error that occurs during parsing of
    * PQL tokens to Query object.
    *
    */
    class SPParseException : public runtime_error {
    public:
        explicit SPParseException(const string& msg = TokeniserConstants::EMPTY_STR) : runtime_error(msg) {}
    };

    /**
    * Represents an error that occurs during validation of
    * PQL Query object.
    *
    */
    class SPValidationException : public runtime_error {
    public:
        explicit SPValidationException(const string& msg = TokeniserConstants::EMPTY_STR) : runtime_error(msg) {}
    };
}

#endif //SPA_SPEXCEPTIONS_H
