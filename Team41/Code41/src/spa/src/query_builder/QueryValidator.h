//
// Created by Xingchen Lin on 31/8/22.
//

#ifndef SPA_QUERYVALIDATOR_H
#define SPA_QUERYVALIDATOR_H

#pragma once

#include "query_builder/commons/Query.h"
#include "query_builder/commons/WithRef.h"
#include <unordered_set>

using namespace std;

namespace QB {
    class QueryValidator {
    private:
        void validateNoDuplicateDeclarations();
        void validateSynonymDeclaredSelectClause();
        void validateSynonymDeclaredSuchThatClause();
        void validateArgRefTypeSuchThatClause();
        void validateUsesModifiesNoUnderscoreForFirstArg();
        void validateSynonymTypeSuchThatClause();
        void validateSuchThatClause();
        void validateSynonymDeclaredPatternClause();
        void validateArgRefTypePatternClause();
        void validateArg1DesignEntityPatternClause();
        void validateArg2DesignEntityPatternClause();
        void validatePatternClause();
        void validateDesignEntityAttrNamePairWithClause();
        void validateSameWithRefWithClause();
        void validateWithClause();
    public:
        //! Validate the Query object, throw Semantic Error if encounter
        shared_ptr<Query> query;
        explicit QueryValidator(shared_ptr<Query> query);
        void validateQuery();
    };
}

#endif //SPA_QUERYVALIDATOR_H
