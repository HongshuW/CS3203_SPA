//
// Created by Nafour on 4/9/22.
//

#ifndef SPA_DUMMYDATARETRIEVER_H
#define SPA_DUMMYDATARETRIEVER_H

#include "pkb/DataRetriever.h"

namespace TestQE {

    class DummyDataRetriever: public DataRetriever {
    public:
        Table getVariables() override;
    };

} // TestQE

#endif //SPA_DUMMYDATARETRIEVER_H
