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
        static const char DOT;
        static const string PQL_VALIDATION_DUPLICATE_SYNONYM_DECLARATION;
        static const string PQL_VALIDATION_SYNONYM_NOT_DECLARED;
        static const string PQL_VALIDATION_ATTR_REF_NOT_DECLARED;
        static const string PQL_VALIDATION_INVALID_REF_TYPES;
        static const string PQL_VALIDATION_FIRST_ARG_UNDERSCORE;
        static const string PQL_VALIDATION_SECOND_ARG_INTEGER;
        static const string PQL_VALIDATION_INVALID_SYNONYM_PATTERN;
        static const string PQL_VALIDATION_INVALID_DESIGN_ENTITY_PATTERN;
        static const string PQL_VALIDATION_INVALID_DESIGN_ENTITY;
        static const string PQL_VALIDATION_INVALID_ATTR_REF;
        static const string PQL_VALIDATION_DIFFERENT_WTIH_REF;
    };

} // QB

#endif //SPA_QUERYVALIDATORCONSTANTS_H
