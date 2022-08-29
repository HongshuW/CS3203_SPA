//
// Created by Xingchen Lin on 28/8/22.
//

#ifndef SPA_QUERY_H
#define SPA_QUERY_H

#pragma once

#include <vector>
#include "query_builder/clauses/Clause.h"
#include "DesignEntity.h"
#include "Synonym.h"
#include "Declaration.h"
#include <unordered_map>

namespace QB {
    class Query {
    public:
        std::vector<Declaration>* declarations;
        std::vector<Clause*>* clauses;

        Query();
        ~Query();

        // For printing
        friend std::ostream& operator<<(std::ostream& os, Query const& query) {
            for (const auto& declaration : *(query.declarations)) {
                os << declaration << "; ";
            }

            for (const auto& clause : *(query.clauses)) {
                os << *clause << " ";
            }

            return os;
        }
    };
}

#endif //SPA_QUERY_H
