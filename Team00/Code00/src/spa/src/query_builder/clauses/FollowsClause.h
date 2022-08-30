//
// Created by Xingchen Lin on 29/8/22.
//

#ifndef SPA_FOLLOWSCLAUSE_H
#define SPA_FOLLOWSCLAUSE_H

#pragma once

#include "query_builder/commons/Synonym.h"
#include "query_builder/clauses/Clause.h"
#include "query_builder/commons/Ref.h"

namespace QB {
    class FollowsClause : public Clause {
    public:
        Ref arg1;
        Ref arg2;
        FollowsClause(Ref arg1, Ref arg2);
        void accept() override;

        bool operator==(const FollowsClause& followsClause) const {
            return arg1 == followsClause.arg1 && arg2 == followsClause.arg2;
        }

        // For printing
        std::ostream & print(std::ostream & os) const {
            // Print the derived class specific information.
            os << "Follows Clause;";
            return os;
        }
    };
}

#endif //SPA_FOLLOWSCLAUSE_H
