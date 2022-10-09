//
// Created by Xingchen Lin on 29/8/22.
//

#ifndef SPA_REF_H
#define SPA_REF_H

#pragma once

#include <iostream>
#include "Synonym.h"
#include "Underscore.h"
#include "Ident.h"
#include <unordered_set>
#include <variant>
#include <unordered_map>
#include "query_builder/constants/CommonConstants.h"

using namespace std;

namespace QB {
    enum class RefType {
        SYNONYM,
        UNDERSCORE,
        INTEGER,
        IDENT
    };

    //! Synonym = 0, Underscore = 1, int = 2, Ident = 3

    using Ref = variant<Synonym, Underscore, int, Ident>;
    using RefTypeSet = unordered_set<unsigned int>;
    static RefTypeSet stmtRefIndexSet = {0, 1, 2};
    static RefTypeSet entRefIndexSet = {0, 1, 3};

    RefType getRefTypeFromIndex(int index);
    RefType getRefType(const Ref& ref);
}

#endif //SPA_REF_H
