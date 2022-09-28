//
// Created by Nafour on 5/9/22.
//

#ifndef SPA_DATAPREPROCESSOR_H
#define SPA_DATAPREPROCESSOR_H

#include <memory>
#include "pkb/DataRetriever.h"
#include "query_builder/commons/DesignEntity.h"
#include "query_builder/clauses/SuchThatClause.h"
#include "query_builder/clauses/pattern_clauses/PatternClause.h"
#include "query_builder/clauses/WithClauses.h"

using namespace std;
using namespace QB;
namespace QE {

    class DataPreprocessor {
        Table getTableByRelationHelper(Ref ref1, Ref ref2, RelationType relationType,
                                       shared_ptr<vector<Declaration>> declarations);
        Table filerTableByColumnValue(const Table& table, const string& colName, const string& value);
        Table filerTableByDesignEntity(const Table& table, const string& colName, DesignEntity designEntity);
        Table filterTableByColValueEquality(Table table, vector<int> comparedCols);
        long getColIndexByColName(vector<string> v, const string& K);
        DesignEntity getDesignEntityOfSyn(Synonym synonym, shared_ptr<vector<Declaration>> declarations);
        shared_ptr<DataRetriever> dataRetriever;

    public:
        explicit DataPreprocessor(shared_ptr<DataRetriever> dataRetriever);
        Table getAllByDesignEntity(DesignEntity designEntity);
        Table getTableByRelation(SuchThatClause suchThatClause);
        Table getTableByPattern(shared_ptr<PatternClause> patternClause);
        Table getTableByWith(shared_ptr<WithClause> withClause, shared_ptr<vector<Declaration>> declarations);

        /**
         * Used for with clause evaluation
         * Valid paras: stmt, read, print, call, while, if assign
         * @param designEntity
         * @return
         */
        vector<int> getStmtNumsByDesignEntity(DesignEntity designEntity);

        /**
         * Used for with clause evaluation
         * Valid paras: procedure, variable, call, read, print
         * @param designEntity
         * @return
         */
         //todo: implement call, read and print
        vector<string> getEntityNames(DesignEntity designEntity);
    };

} // QE

#endif //SPA_DATAPREPROCESSOR_H
