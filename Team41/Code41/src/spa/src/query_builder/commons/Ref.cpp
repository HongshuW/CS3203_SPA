//
// Created by Xingchen Lin on 31/8/22.
//

#include "Ref.h"
#include "query_builder/exceptions/Exceptions.h"

namespace QB {

    unordered_map<int, RefType> indexToRefTypeMap({
        {CommonConstants::ZERO, RefType::SYNONYM},
        {CommonConstants::ONE, RefType::UNDERSCORE},
        {CommonConstants::TWO, RefType::INTEGER},
        {CommonConstants::THREE, RefType::IDENT}
    });


    RefType getRefTypeFromIndex(int index) {
        try {
            return indexToRefTypeMap.at(index);
        } catch (const std::out_of_range& oor) {
            throw PQLParseException(CommonConstants::PQL_INVALID_REF_TYPE);
        }
    }

    RefType getRefType(const Ref& ref) {
        return getRefTypeFromIndex((int) ref.index());
    }
}