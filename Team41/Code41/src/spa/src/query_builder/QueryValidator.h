//
// Created by Xingchen Lin on 31/8/22.
//

#ifndef SPA_QUERYVALIDATOR_H
#define SPA_QUERYVALIDATOR_H

#pragma once

#include "query_builder/commons/Query.h"
#include "query_builder/commons/WithRef.h"
#include "query_builder/constants/QueryValidatorConstants.h"
#include <unordered_set>

using namespace std;

namespace QB {
    class QueryValidator {
    private:
        void validateNoDuplicateDeclarations() const;
        void validateSelectClause();
        void validateSynonymDeclaredSelectClause() const;
        void validateDesignEntityAttrNamePairSelectClause() const;
        void validateSynonymDeclaredSuchThatClause() const;
        void validateArgRefTypeSuchThatClause() const;
        void validateUsesModifiesNoUnderscoreForFirstArg() const;
        void validateSynonymTypeSuchThatClause() const;
        void validateSuchThatClause();
        void validateSynonymDeclaredPatternClause() const;
        void validateArgRefTypePatternClause() const;
        void validateArg1DesignEntityPatternClause() const;
        void validateArg2DesignEntityPatternClause() const;
        void validatePatternClause();
        void validateDesignEntityAttrNamePairWithClause() const;
        void validateSameWithRefWithClause() const;
        void validateWithClause();

    public:
        //! Validate the Query object, throw Semantic Error if encounter
        shared_ptr<Query> query;
        explicit QueryValidator(shared_ptr<Query> query);
        void validateQuery();
    };
}

#endif //SPA_QUERYVALIDATOR_H
