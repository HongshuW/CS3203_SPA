//
// Created by Nafour on 11/9/22.
//

#include "Utils.h"

using namespace QB;
namespace QE {
    unordered_map<QB::RefType, string> Utils::refTypeToColNameMap({
                                                                          {RefType::INTEGER, "$integer_col"},
                                                                          {RefType::IDENT, "$ident_col"},
                                                                          {RefType::UNDERSCORE, "$underscore_col"}
                                                                  });
    string Utils::getColNameByRefType(QB::RefType refType) {
        return refTypeToColNameMap.at(refType);
    }
} // QE