////
//// Created by Nafour on 8/9/22.
////
//
//#ifndef SPA_DUMMYDATARETRIEVER2_H
//#define SPA_DUMMYDATARETRIEVER2_H
//
//#include "pkb/DataRetriever.h"
//
///*
// * procedure p2 {
// *  1  read x;
// *  2  read y;
// *  3  while ( y > 0 ) {
// *  4        x = x + 1
// *  5        if ( x > 2 ) {
// *  6            x + 2
// *          }
// *  7       y = y - 1
// *    }
// *  8  print x
// * }
// */
//
//class DummyDataRetriever2 : public DataRetriever {
//public:
//     Table getTableByDesignEntity(DesignEntity designEntity) override;
//     Table getTableByRelationType(RelationType relationType) override;
//     DesignEntity getDesignEntityOfStmt(int stmtNumber) override;
//     explicit DummyDataRetriever2(shared_ptr<PKBStorage> pkbStorage);
//};
//
//
//#endif //SPA_DUMMYDATARETRIEVER2_H
