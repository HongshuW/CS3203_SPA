//
// Created by Xingchen Lin on 27/8/22.
//

#ifndef SPA_EXCEPTIONS_H
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
}

#endif //SPA_EXCEPTIONS_H
