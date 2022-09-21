//
// Created by Xingchen Lin on 21/9/22.
//

#include "WithRef.h"

namespace QB {
    unordered_map<int, WithRefType> indexToWithRefTypeMap({
        {0, WithRefType::IDENT},
        {1, WithRefType::INTEGER},
        {2, WithRefType::ATTR_REF}
    });

    WithRefType getWithRefTypeFromIndex(int index) {
        try {
            return indexToWithRefTypeMap.at(index);
        } catch (const std::out_of_range& oor) {
            throw PQLParseException("Cannot find the WithRefType at index " + index);
        }
    }
}