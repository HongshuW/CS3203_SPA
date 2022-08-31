//
// Created by Xingchen Lin on 31/8/22.
//

#ifndef SPA_RELATIONTYPE_H
#define SPA_RELATIONTYPE_H

#pragma once

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

namespace QB {
    enum class RelationType {
        FOLLOWS,
        FOLLOWS_T,
        PARENT,
        PARENT_T,
        MODIFIES,
        USES
    };

    RelationType getRelationTypeFromStr(string& str);
    string getStrFromRelationType(RelationType relationType);
}

#endif //SPA_RELATIONTYPE_H
