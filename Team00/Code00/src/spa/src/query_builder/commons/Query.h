//
// Created by Xingchen Lin on 28/8/22.
//

#ifndef SPA_QUERY_H
#define SPA_QUERY_H

#pragma once

#include <vector>
#include "query_builder/clauses/SuchThatClause.h"
#include "query_builder/clauses/SelectClause.h"
#include "DesignEntity.h"
#include "Synonym.h"
#include "Declaration.h"
#include <unordered_map>

using namespace std;

namespace QB {
    class Query {
    public:
        vector<Declaration>* declarations;
        SelectClause* selectClause;
        vector<SuchThatClause*>* suchThatClauses;

        Query();
        ~Query();

        // For printing
        friend ostream& operator<<(ostream& os, Query const& query) {
            for (const auto& declaration : *(query.declarations)) {
                os << declaration << "; ";
            }

            os << query.selectClause << "; ";

            for (const auto& clause : *(query.suchThatClauses)) {
                os << clause << " ";
            }

            return os;
        }
    };
}

#endif //SPA_QUERY_H
