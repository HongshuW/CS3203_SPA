//
// Created by Nafour on 5/9/22.
//

#ifndef SPA_DATAPREPROCESSOR_H
#define SPA_DATAPREPROCESSOR_H

#include <memory>
#include "pkb/DataRetriever.h"
#include "query_builder/commons/DesignEntity.h"
#include "query_builder/clauses/SuchThatClause.h"
#include "FakeDataRetriever.h"

using namespace std;
using namespace QB;
namespace QE {

    class DataPreprocessor {
        Table getRelationTable(Ref ref1, Ref ref2, RelationType relationType);
        Table filerTableByColumnValue(const Table& table, const string& colName, const string& value);
        Table filerTableByDesignEntity(const Table& table, const string& colName, DesignEntity designEntity);
        long getIndex(vector<string> v, const string& K);
        bool is_number(const std::string& s);
        DesignEntity getDesignEntity(Synonym synonym, shared_ptr<vector<Declaration>> declarations);
    public:
        shared_ptr<DataRetriever> dataRetriever;
        Table getAllByDesignEntity(DesignEntity designEntity);
        Table getTableByRelation(SuchThatClause suchThatClause);
        explicit DataPreprocessor(shared_ptr<FakeDataRetriever> dataRetriever);

    };

} // QE

#endif //SPA_DATAPREPROCESSOR_H
