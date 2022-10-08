//
// Created by Nafour on 5/9/22.
//

#ifndef SPA_DUMMYDATARETRIEVER_H
#define SPA_DUMMYDATARETRIEVER_H

#include "pkb/DataRetriever.h"
#include "pkb/table/Table.h"

class DummyDataRetriever : public DataRetriever {
    shared_ptr<PKBStorage> pkbStorage;

public:
//    Table getTableByRelationType(QB::RelationType relationType) override;
    Table getTableByDesignEntity(DesignEntity designEntity) override;
//    Table getTableByExprPattern(ExpressionSpec expressionSpec) override;
    Table getFollowsTable() override;
    Table getFollowsTTable() override;
    Table getParentTable() override;
    Table getParentTTable() override;
    Table getUsesSTable() override;
    Table getUsesPTable() override;
    Table getModifiesSTable() override;
    Table getModifiesPTable() override;
    Table getCallsTable() override;
    Table getCallsTTable() override;
    Table getNextTable() override;
    Table getNextTTable() override;
    Table getAffectsTable() override;
    Table getAffectsTTable() override;
    DesignEntity getDesignEntityOfStmt(int line_no) override;
    explicit DummyDataRetriever(shared_ptr<PKBStorage> pkbStorage);
//    explicit DummyDataRetriever();
};


#endif //SPA_DUMMYDATARETRIEVER_H