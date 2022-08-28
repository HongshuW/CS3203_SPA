//
// Created by Xingchen Lin on 27/8/22.
//

#ifndef SPA_QUERY_BUILDER_H
#define SPA_QUERY_BUILDER_H

#pragma once

#include <string>
#include <vector>

namespace QB {

    class QueryBuilder {
    public:
        /**
         * Takes a PQL query string and convert it to a Query object.
         *
         * @param string PQL query string.
         * @return Query object containing the query information.
         */
        // TODO: to be implemented later, change std::vector<std::string> to Query
        std::vector<std::string> buildPQLQuery(std::string& query);
    };
}

#endif