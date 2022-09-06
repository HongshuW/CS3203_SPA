//
// Created by Xingchen Lin on 31/8/22.
//

#include "Ref.h"
#include "query_builder/Exceptions.h"

namespace QB {
    unordered_map<RefType, int> refTypeToIndexMap({
        {RefType::SYNONYM, 0},
        {RefType::UNDERSCORE, 1},
        {RefType::INTEGER, 2},
        {RefType::IDENT, 3}
    });

    unordered_map<int, RefType> indexToRefTypeMap({
        {0, RefType::SYNONYM},
        {1, RefType::UNDERSCORE},
        {2, RefType::INTEGER},
        {3, RefType::IDENT}
    });

    //! For printing
    unordered_map<RefType, string> refTypeToStringMap({
        {RefType::SYNONYM, "Synonym"},
        {RefType::UNDERSCORE, "Underscore"},
        {RefType::INTEGER, "Integer"},
        {RefType::IDENT, "Ident"}
    });

    RefType getRefTypeFromIndex(int index) {
        try {
            return indexToRefTypeMap.at(index);
        } catch (const std::out_of_range& oor) {
            throw PQLParseException("Cannot find the RefType at index " +
                                            index);
        }
    }

    RefType getRefType(Ref ref) {
        return getRefTypeFromIndex(ref.index());
    }

    int getIndexFromRefType(RefType refType) {
        return refTypeToIndexMap.at(refType);
    }

    string refTypeToString(RefType refType) {
        return refTypeToStringMap.at(refType);
    }
}