//
// Created by Nafour on 20/9/22.
//

#ifndef SPA_QUERYBOOLEANRESULT_H
#define SPA_QUERYBOOLEANRESULT_H

#include "QueryResult.h"

namespace QE {

    class QueryBooleanResult : public QueryResult {


    public:
        bool boolVal;
        explicit QueryBooleanResult(bool boolVal);

    };

} // QE

#endif //SPA_QUERYBOOLEANRESULT_H
