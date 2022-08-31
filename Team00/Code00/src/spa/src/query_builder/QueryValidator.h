//
// Created by Xingchen Lin on 31/8/22.
//

#ifndef SPA_QUERYVALIDATOR_H
#define SPA_QUERYVALIDATOR_H

#pragma once

#include "query_builder/commons/Query.h"

namespace QB {
    class QueryValidator {
    private:
        void validateNoDuplicateDeclarations();
        void validateSynonymDeclaredSelectClause();
        void validateSynonymDeclaredSuchThatClause();
        void validateArgRefTypeSuchThatClause();
        void validateSuchThatClause();
    public:
        Query* query;
        explicit QueryValidator(Query* query);
        void validateQuery();
    };
}

#endif //SPA_QUERYVALIDATOR_H
