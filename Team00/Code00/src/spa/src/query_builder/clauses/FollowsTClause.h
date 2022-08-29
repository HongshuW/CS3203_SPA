//
// Created by Xingchen Lin on 30/8/22.
//

#ifndef SPA_FOLLOWSTCLAUSE_H
#define SPA_FOLLOWSTCLAUSE_H

#pragma once

#include "query_builder/commons/Synonym.h"
#include "query_builder/clauses/Clause.h"
#include "query_builder/commons/Ref.h"

namespace QB {
    class FollowsTClause : public Clause {
    public:
        Ref arg1;
        Ref arg2;
        FollowsTClause(Ref arg1, Ref arg2);
        void accept() override;

        bool operator==(const FollowsTClause& followsClause) const {
            return arg1 == followsClause.arg1 && arg2 == followsClause.arg2;
        }

        // For printing
        std::ostream & print(std::ostream & os) const {
            // Print the derived class specific information.
            os << "FollowsT Clause;";
            return os;
        }
    };
}

#endif //SPA_FOLLOWSTCLAUSE_H
