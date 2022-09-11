//
// Created by Xingchen Lin on 27/8/22.
//

#ifndef SPA_SPEXCEPTIONS_H
#define SPA_EXCEPTIONS_H

#pragma once
#include <string>

namespace QB {

    /**
     * Represents an error that occurs during tokenization of
     * PQL query string.
     *
     */
    class PQLTokenizeException : public std::runtime_error {
    public:
        PQLTokenizeException(const std::string& msg = "") : std::runtime_error(msg) {}
    };

    /**
    * Represents an error that occurs during parsing of
    * PQL tokens to Query object.
    *
    */
    class PQLParseException : public std::runtime_error {
    public:
        PQLParseException(const std::string& msg = "") : std::runtime_error(msg) {}
    };

    /**
    * Represents an error that occurs during validation of
    * PQL Query object.
    *
    */
    class PQLValidationException : public std::runtime_error {
    public:
        PQLValidationException(const std::string& msg = "") : std::runtime_error(msg) {}
    };
}

#endif //SPA_SPEXCEPTIONS_H
