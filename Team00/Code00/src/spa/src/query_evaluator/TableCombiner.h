//
// Created by Nafour on 6/9/22.
//

#ifndef SPA_TABLECOMBINER_H
#define SPA_TABLECOMBINER_H

#include "pkb/Table.h"
#include <unordered_map>
namespace QE {

    class TableCombiner {
        /**
         * @return pairs of duplicate headers' index in the cross product table
         */
        vector<vector<int>> findDuplicateHeaders(vector<string> h1, vector<string> h2);

        Table crossProduct(const Table& t1, const Table& t2);
    public:
        Table joinTable(const Table &t1, const Table &t2);

    };

} // QE

#endif //SPA_TABLECOMBINER_H
