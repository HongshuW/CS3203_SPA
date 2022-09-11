//
// Created by Nafour on 11/9/22.
//

#ifndef SPA_DUMMYQUERIES_H
#define SPA_DUMMYQUERIES_H

#include "query_builder/clauses/SuchThatClause.h"
namespace TestQE {

    class DummyQueries {
    public:
        static SuchThatClause getDummySuchThat(int idx);

    };

} // TestQE

#endif //SPA_DUMMYQUERIES_H
