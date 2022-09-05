//
// Created by Nafour on 5/9/22.
//

#ifndef SPA_DUMMYDATARETRIEVER_H
#define SPA_DUMMYDATARETRIEVER_H

#include "pkb/DataRetriever.h"
#include "query_builder/commons/RelationType.h"
using namespace QB;
namespace QE {

    class DummyDataRetriever: public DataRetriever {
    public:
        Table getTableByRelationType(RelationType relationType);
    };

} // QE

#endif //SPA_DUMMYDATARETRIEVER_H
