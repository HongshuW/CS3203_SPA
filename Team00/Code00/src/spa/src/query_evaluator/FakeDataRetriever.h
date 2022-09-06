//
// Created by Nafour on 5/9/22.
//

#ifndef SPA_FAKEDATARETRIEVER_H
#define SPA_FAKEDATARETRIEVER_H

#include "pkb/Table.h"
#include "query_builder/commons/RelationType.h"
#include "pkb/DataRetriever.h"

using namespace QB;
namespace QE {

    class FakeDataRetriever : public DataRetriever {
    public:
        virtual Table getTableByRelationType(RelationType relationType);
    };

} // QE

#endif //SPA_FAKEDATARETRIEVER_H
