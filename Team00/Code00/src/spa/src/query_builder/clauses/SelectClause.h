//
// Created by Xingchen Lin on 29/8/22.
//

#ifndef SPA_SELECTCLAUSE_H
#define SPA_SELECTCLAUSE_H

#pragma once

#include "query_builder/commons/Synonym.h"

namespace QB {
    class SelectClause {
    public:
        Synonym synonym;
        explicit SelectClause(Synonym synonym);

        bool operator==(const SelectClause& selectClause1) const { return synonym == selectClause1.synonym; }

        // For printing
        std::ostream & print(std::ostream & os) const {
            // Print the derived class specific information.
            os << "Select Clause, synonym: " << synonym << ";";
            return os;
        }
    };
}

#endif //SPA_SELECTCLAUSE_H
