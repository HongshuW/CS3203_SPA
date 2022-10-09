//
// Created by Xingchen Lin on 31/8/22.
//

#ifndef SPA_RELATIONTYPE_H
#define SPA_RELATIONTYPE_H

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "DesignEntity.h"
#include "Ref.h"

using namespace std;
using namespace QB;

namespace QB {
    enum class RelationType {
        FOLLOWS,
        FOLLOWS_T,
        PARENT,
        PARENT_T,
        MODIFIES,
        MODIFIES_S,
        MODIFIES_P,
        USES,
        USES_S,
        USES_P,
        CALLS,
        CALLS_T,
        NEXT,
        NEXT_T,
        AFFECTS,
        AFFECTS_T
    };

    RelationType getRelationTypeFromStr(string& str);
}

#endif //SPA_RELATIONTYPE_H
