//
// Created by Nafour on 5/9/22.
//

#ifndef SPA_DATAPREPROCESSOR_H
#define SPA_DATAPREPROCESSOR_H

#include <memory>
#include "pkb/DataRetriever.h"
#include "query_builder/commons/DesignEntity.h"
#include "query_builder/clauses/SuchThatClause.h"
#include "DummyDataRetriever.h"

using namespace std;
using namespace QB;
namespace QE {

    class DataPreprocessor {
        Table getRelationTable(Ref ref1, Ref ref2, RelationType relationType);
        Table filerTableByColumnValue(const Table& table, const string& colName, const string& value);
        long getIndex(vector<string> v, const string& K);
    public:
        shared_ptr<DummyDataRetriever> dataRetriever;
        Table getAllByDesignEntity(DesignEntity designEntity);
        Table getTableByRelation(SuchThatClause suchThatClause);
        explicit DataPreprocessor(shared_ptr<DummyDataRetriever> dataRetriever);
    };

} // QE

#endif //SPA_DATAPREPROCESSOR_H
