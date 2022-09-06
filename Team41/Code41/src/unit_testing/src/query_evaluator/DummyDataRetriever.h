//
// Created by Nafour on 5/9/22.
//

#ifndef SPA_DUMMYDATARETRIEVER_H
#define SPA_DUMMYDATARETRIEVER_H

#include "query_evaluator/FakeDataRetriever.h"

class DummyDataRetriever : public QE::FakeDataRetriever {
    public:
        Table getVariables() override;
        Table getTableByRelationType(QB::RelationType relationType) override;
        DummyDataRetriever();
};


#endif //SPA_DUMMYDATARETRIEVER_H
