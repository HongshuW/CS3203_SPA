//
// Created by Xingchen Lin on 1/10/22.
//

#ifndef SPA_VALIDATABLE_H
#define SPA_VALIDATABLE_H

#include "query_builder/commons/Ref.h"
#include "query_builder/commons/DesignEntity.h"
#include "query_builder/clauses/AbstractClause.h"

namespace QB {
    class Validatable {
    public:
        virtual pair<unordered_set<DesignEntity>, unordered_set<DesignEntity>> getAllowedArgsSynonym() = 0;
        virtual pair<RefTypeSet, RefTypeSet> getAllowedArgsRefType() = 0;
    };
}

#endif //SPA_VALIDATABLE_H
