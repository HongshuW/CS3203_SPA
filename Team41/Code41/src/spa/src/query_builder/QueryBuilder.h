//
// Created by Xingchen Lin on 27/8/22.
//

#ifndef SPA_QUERYBUILDER_H
#define SPA_QUERYBUILDER_H

#pragma once

#include <string>
#include <vector>
#include "query_builder/commons/Query.h"

namespace QB {

    class QueryBuilder {
    public:
        /**
         * Takes a PQL query string and convert it to a query object.
         *
         * @param string PQL query string.
         * @return a shared pointer to the query object containing the query information.
         */
        shared_ptr<Query> buildPQLQuery(string& queryStr);
    };
}

#endif