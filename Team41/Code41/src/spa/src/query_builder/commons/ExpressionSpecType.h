//
// Created by Xingchen Lin on 11/9/22.
//

#ifndef SPA_EXPRESSIONSPECTYPE_H
#define SPA_EXPRESSIONSPECTYPE_H

#pragma once

namespace QB {
enum class ExpressionSpecType {
  ANY_MATCH,      // _
  PARTIAL_MATCH,  // _"x"_
  FULL_MATCH      // "x"
};
}

#endif  // SPA_EXPRESSIONSPECTYPE_H
