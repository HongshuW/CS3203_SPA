//
// Created by Tan Wei En on 8/9/22.
//

#ifndef SPA_SPEXCEPTIONS_H
#define SPA_SPEXCEPTIONS_H

#pragma once
#include <stdexcept>
#include <string>

#include "constants/TokenizerConstants.h"
using namespace std;

namespace SourceParser {
/**
 * Represents an error that occurs during tokenization of
 * source program.
 *
 */
class SPTokenizeException : public runtime_error {
 public:
  explicit SPTokenizeException(
      const string& msg = TokenizerConstants::EMPTY_STR)
      : runtime_error(msg) {}
};

/**
 * Represents an error that occurs during parsing of
 * source program.
 *
 */
class SPParseException : public runtime_error {
 public:
  explicit SPParseException(const string& msg = TokenizerConstants::EMPTY_STR)
      : runtime_error(msg) {}
};

/**
 * Represents an error that occurs during validation of
 * AST produced by parser
 *
 */
class SPValidationException : public runtime_error {
 public:
  explicit SPValidationException(
      const string& msg = TokenizerConstants::EMPTY_STR)
      : runtime_error(msg) {}
};
}  // namespace SourceParser

#endif  // SPA_SPEXCEPTIONS_H
