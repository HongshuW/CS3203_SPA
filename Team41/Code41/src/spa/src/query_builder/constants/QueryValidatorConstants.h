//
// Created by Xingchen Lin on 28/9/22.
//

#ifndef SPA_QUERYVALIDATORCONSTANTS_H
#define SPA_QUERYVALIDATORCONSTANTS_H

#pragma once

#include <string>

using namespace std;

namespace QB {
    class QueryValidatorConstants {
    public:
        static const string PQL_VALIDATION_DUPLICATE_SYNONYM_DECLARATION;
        static const string PQL_VALIDATION_SYNONYM_NOT_DECLARED;
        static const string PQL_VALIDATION_ATTRREF_NOT_DECLARED;
    };

} // QB

#endif //SPA_QUERYVALIDATORCONSTANTS_H
