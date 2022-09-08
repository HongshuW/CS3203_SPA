//
// Created by Nafour on 5/9/22.
//

#ifndef SPA_DUMMYDATARETRIEVER_H
#define SPA_DUMMYDATARETRIEVER_H

#include "pkb/DataRetriever.h"
#include "pkb/table/Table.h"

class DummyDataRetriever : public DataRetriever {
public:
    Table getVariables() override;
    Table getTableByRelationType(QB::RelationType relationType) override;
    DummyDataRetriever();
};


#endif //SPA_DUMMYDATARETRIEVER_H