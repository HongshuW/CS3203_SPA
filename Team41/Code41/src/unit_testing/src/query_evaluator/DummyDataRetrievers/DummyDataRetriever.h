//
// Created by Nafour on 5/9/22.
//

#ifndef SPA_DUMMYDATARETRIEVER_H
#define SPA_DUMMYDATARETRIEVER_H

#include "pkb/DataRetriever.h"
#include "pkb/table/Table.h"

class DummyDataRetriever : public DataRetriever {
public:
    Table getTableByRelationType(QB::RelationType relationType) override;
    Table getTableByDesignEntity(DesignEntity designEntity) override;
    DesignEntity getDesignEntityOfStmt(int line_no) override;
    DummyDataRetriever();
};


#endif //SPA_DUMMYDATARETRIEVER_H