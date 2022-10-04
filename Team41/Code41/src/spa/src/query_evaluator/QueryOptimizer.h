//
// Created by Nafour on 4/10/22.
//

#ifndef SPA_QUERYOPTIMIZER_H
#define SPA_QUERYOPTIMIZER_H
#include <unordered_set>
#include "query_builder/clauses/AbstractClause.h"
#include "query_builder/commons/Query.h"

using namespace  std;
namespace QE {

    class QueryOptimizer {
        shared_ptr<Query> query;
        int clauseCount;
        vector<int> parent;
        vector<pair<int, int>> edges;
        unordered_map<shared_ptr<AbstractClause>, int> clauseIdMap;

        void initParent();
        void initclauseIdMap();
        void initEdges();

    public:
        explicit QueryOptimizer(shared_ptr<Query> query);
        unordered_set<shared_ptr<AbstractClause>> optimise();
    };

} // QE

#endif //SPA_QUERYOPTIMIZER_H
