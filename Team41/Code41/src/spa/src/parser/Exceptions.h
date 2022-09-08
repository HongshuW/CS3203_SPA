//
// Created by Tan Wei En on 8/9/22.
//

#ifndef SPA_EXCEPTIONS_H
#define SPA_EXCEPTIONS_H

#pragma once
#include <string>

using namespace std;

namespace SourceParser {
    /**
     * Represents an error that occurs during tokenization of
     * PQL query string.
     *
     */
    class SPTokenizeException : public runtime_error {
    public:
        SPTokenizeException(const string& msg = "") : runtime_error(msg) {}
    };

    /**
    * Represents an error that occurs during parsing of
    * PQL tokens to Query object.
    *
    */
    class SPParseException : public runtime_error {
    public:
        SPParseException(const string& msg = "") : runtime_error(msg) {}
    };

    /**
    * Represents an error that occurs during validation of
    * PQL Query object.
    *
    */
    class SPValidationException : public runtime_error {
    public:
        SPValidationException(const string& msg = "") : runtime_error(msg) {}
    };
}

#endif //SPA_EXCEPTIONS_H
