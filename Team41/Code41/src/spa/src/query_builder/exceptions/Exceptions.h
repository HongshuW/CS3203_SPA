//
// Created by Xingchen Lin on 27/8/22.
//

#ifndef SPA_EXCEPTIONS_H
#define SPA_EXCEPTIONS_H

#pragma once
#include <stdexcept>
#include <string>

namespace QB {

/**
 * Represents an error that occurs during tokenization of
 * PQL query string.
 *
 */
class PQLTokenizeException : public std::runtime_error {
 public:
  explicit PQLTokenizeException(const std::string& msg = "")
      : std::runtime_error(msg) {}
};

/**
 * Represents an error that occurs during parsing of
 * PQL tokens to Query object.
 *
 */
class PQLParseException : public std::runtime_error {
 public:
  explicit PQLParseException(const std::string& msg = "")
      : std::runtime_error(msg) {}
};

/**
 * Represents an error that occurs during validation of
 * PQL Query object.
 *
 */
class PQLValidationException : public std::runtime_error {
 public:
  explicit PQLValidationException(const std::string& msg = "")
      : std::runtime_error(msg) {}
};
}  // namespace QB

#endif  // SPA_EXCEPTIONS_H
