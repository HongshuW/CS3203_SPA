//
// Created by Xingchen Lin on 21/9/22.
//

#include "WithRef.h"

namespace QB {

    unordered_map<int, WithRefType> indexToWithRefTypeMap({
        {CommonConstants::ZERO, WithRefType::IDENT},
        {CommonConstants::ONE, WithRefType::INTEGER},
        {CommonConstants::TWO, WithRefType::ATTR_REF}
    });

    WithRefType getWithRefTypeFromIndex(int index) {
        try {
            return indexToWithRefTypeMap.at(index);
        } catch (const std::out_of_range& oor) {
            throw PQLParseException(CommonConstants::PQL_INVALID_WITH_REF_TYPE);
        }
    }
}