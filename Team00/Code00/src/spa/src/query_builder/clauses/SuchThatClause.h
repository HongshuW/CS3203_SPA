//
// Created by Xingchen Lin on 29/8/22.
//

#ifndef SPA_SUCHTHATCLAUSE_H
#define SPA_SUCHTHATCLAUSE_H

#pragma once

#include "query_builder/commons/Synonym.h"
#include "query_builder/commons/Ref.h"
#include "query_builder/commons/RelationType.h"

namespace QB {
    class SuchThatClause {
    public:
        RelationType relationType;
        Ref arg1;
        Ref arg2;
        SuchThatClause(RelationType relationType, Ref arg1, Ref arg2);

        bool operator==(const SuchThatClause& followsClause) const {
            return arg1 == followsClause.arg1 && arg2 == followsClause.arg2;
        }

        // For printing
        std::ostream & print(std::ostream & os) const {
            // Print the derived class specific information.
            os << getStrFromRelationType(relationType) << "Clause;";
            return os;
        }
    };
}

#endif //SPA_SUCHTHATCLAUSE_H
