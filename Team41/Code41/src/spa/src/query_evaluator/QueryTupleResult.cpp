//
// Created by Nafour on 20/9/22.
//

#include "QueryTupleResult.h"

namespace QE {
    QueryTupleResult::QueryTupleResult(shared_ptr<vector<Elem>> returnTuple,
                                       shared_ptr<vector<Declaration>> declarations) : returnTuple(returnTuple), declarations(declarations){

    }
} // QE