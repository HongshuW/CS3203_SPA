//
// Created by Nafour on 11/9/22.
//

#include "QEUtils.h"

using namespace QB;
namespace QE {
    unordered_map<QB::RefType, string> QEUtils::refTypeToColNameMap({
                                                                          {RefType::INTEGER, "$integer_col"},
                                                                          {RefType::IDENT, "$ident_col"},
                                                                          {RefType::UNDERSCORE, "$underscore_col"}
                                                                  });
    string QEUtils::getColNameByRefType(QB::RefType refType) {
        return refTypeToColNameMap.at(refType);
    }

    Table QEUtils::getScalarResponse(bool hasResult) {
        const string TRUE = "TRUE";
        const string FALSE = "FALSE";

        const string HEADER = "$dummy_header";
        const string VALUE = hasResult ? TRUE : FALSE ;
        Table resultTable = Table();
        resultTable.renameHeader({HEADER}) ;
        resultTable.rows = vector<vector<string>>({{VALUE}});
        return resultTable;
    }

} // QE