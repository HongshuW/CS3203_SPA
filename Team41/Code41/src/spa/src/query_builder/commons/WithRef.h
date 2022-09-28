//
// Created by Xingchen Lin on 21/9/22.
//

#ifndef SPA_WITHREF_H
#define SPA_WITHREF_H

#pragma once

#include "Ident.h"
#include "AttrRef.h"
#include <unordered_map>
#include <variant>

namespace QB {
    enum class WithRefType {
        IDENT,
        INTEGER,
        ATTR_REF
    };
    using WithRef = variant<Ident, int, AttrRef>;
    WithRefType getWithRefTypeFromIndex(int index);
} // QB

#endif //SPA_WITHREF_H
