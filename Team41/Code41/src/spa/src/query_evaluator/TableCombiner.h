//
// Created by Nafour on 6/9/22.
//

#ifndef SPA_TABLECOMBINER_H
#define SPA_TABLECOMBINER_H

#include "pkb/table/Table.h"
#include <unordered_map>
namespace QE {

    class TableCombiner {
        /**
         * @return pairs of duplicate headers' index in the cross product table
         */
        vector<vector<int>> findDuplicateHeaders(vector<string> h1, vector<string> h2);

    public:
        Table crossProduct(const Table &t1, const Table &t2);
        Table joinTable(const Table &t1, const Table &t2);


    };

} // QE

#endif //SPA_TABLECOMBINER_H
