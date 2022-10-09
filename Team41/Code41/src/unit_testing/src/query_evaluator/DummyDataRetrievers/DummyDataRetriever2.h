//
// Created by Nafour on 8/9/22.
//

#ifndef SPA_DUMMYDATARETRIEVER2_H
#define SPA_DUMMYDATARETRIEVER2_H

#include "pkb/DataRetriever.h"

/*
 * procedure p2 {
 *  1  read x;
 *  2  read y;
 *  3  while ( y > 0 ) {
 *  4        x = x + 1
 *  5        if ( x > 2 ) {
 *  6            x + 2
 *          }
 *  7       y = y - 1
 *    }
 *  8  print x
 * }
 */

class DummyDataRetriever2 : public DataRetriever {
public:
     Table getTableByDesignEntity(DesignEntity designEntity) override;
     DesignEntity getDesignEntityOfStmt(int stmtNumber) override;
     Table getFollowsTable() override;
     Table getFollowsTTable() override;
     Table getParentTable() override;
     Table getParentTTable() override;
     Table getUsesSTable() override;
//     Table getUsesPTable() override;
     Table getModifiesSTable() override;
//     Table getModifiesPTable() override;
//     Table getCallsTable() override;
//     Table getCallsTTable() override;
//     Table getNextTable() override;
//     Table getNextTTable() override;
//     Table getAffectsTable() override;
//     Table getAffectsTTable() override;
//     Table getAssignPatternTable(ExpressionSpec expressionSpec) override;
     Table getIfPatternTable() override;
     Table getWhilePatternTable() override;
     explicit DummyDataRetriever2(shared_ptr<PKBStorage> pkbStorage);
};


#endif //SPA_DUMMYDATARETRIEVER2_H
