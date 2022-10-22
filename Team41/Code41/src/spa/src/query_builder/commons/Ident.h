//
// Created by Xingchen Lin on 4/9/22.
//

#ifndef SPA_IDENT_H
#define SPA_IDENT_H

#pragma once

#include <iostream>
#include <optional>
#include <regex>

#include "query_builder/constants/CommonConstants.h"
#include "utils/Utils.h"

using namespace std;

namespace QB {
class Ident {
 public:
  string identStr;
  explicit Ident(const string& identStr);

  bool operator==(const Ident& ident1) const;
};
}  // namespace QB

#endif  // SPA_IDENT_H
