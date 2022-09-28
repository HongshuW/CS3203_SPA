//
// Created by Xingchen Lin on 29/8/22.
//

#ifndef SPA_SUCHTHATCLAUSE_H
#define SPA_SUCHTHATCLAUSE_H

#pragma once

#include "query_builder/commons/Synonym.h"
#include "query_builder/commons/Ref.h"
#include "query_builder/commons/RelationType.h"
#include "query_builder/commons/Declaration.h"

#include <ostream>

namespace QB {
    class SuchThatClause {
    private:
        //! Update specific relationship lhsType for Modifies (Modifies_S / Modifies_P) Clause
        //! and Uses Clause (Uses_S, Uses_P)
        void updateSpecificModifiesUsesClause();
    public:
        RelationType relationType;
        Ref arg1;
        Ref arg2;
        shared_ptr<vector<Declaration>> declarations;
        SuchThatClause(RelationType relationType, Ref arg1, Ref arg2, shared_ptr<vector<Declaration>> declarations);

        bool operator==(const SuchThatClause& clause) const;
        std::ostream & print(std::ostream & os) const;
    };
}

#endif //SPA_SUCHTHATCLAUSE_H
