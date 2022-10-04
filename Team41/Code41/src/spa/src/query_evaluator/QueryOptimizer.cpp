//
// Created by Nafour on 4/10/22.
//

#include "QueryOptimizer.h"

namespace QE {
    QueryOptimizer::QueryOptimizer(shared_ptr<Query> query) {

    }

    void QueryOptimizer::initParent() {
        parent = vector<int>(clauseCount);
        for (int i = 0; i < clauseCount; i++) {
            parent[i] = i;
        }
    }

} // QE