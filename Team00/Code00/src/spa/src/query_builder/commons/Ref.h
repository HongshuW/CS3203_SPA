//
// Created by Xingchen Lin on 29/8/22.
//

#ifndef SPA_REF_H
#define SPA_REF_H

#pragma once

#include <iostream>
#include "Synonym.h"
#include "Underscore.h"
#include <unordered_set>
#include <variant>
#include <unordered_map>

using namespace std;

namespace QB {
    enum class RefType {
        SYNONYM,
        UNDERSCORE,
        INTEGER,
        STRING
    };

    //! Synonym = 0, Underscore = 1, int = 2, string = 3
    using Ref = variant<Synonym, Underscore, int, string>;

    RefType getRefTypeFromIndex(int index);
    int getIndexFromRefType(RefType refType);
    RefType getRefType(Ref ref);
    //! For printing
    string refTypeToString(RefType refType);
}

#endif //SPA_REF_H
