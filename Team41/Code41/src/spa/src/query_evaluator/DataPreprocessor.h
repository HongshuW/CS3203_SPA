//
// Created by Nafour on 5/9/22.
//

#ifndef SPA_DATAPREPROCESSOR_H
#define SPA_DATAPREPROCESSOR_H

#include <memory>
#include "pkb/DataRetriever.h"
#include "query_builder/commons/DesignEntity.h"
#include "query_builder/clauses/such_that_clauses/SuchThatClause.h"
#include "query_builder/clauses/pattern_clauses/PatternClause.h"
#include "query_builder/clauses/with_clauses/WithClauses.h"
#include "query_builder/commons/Declaration.h"

using namespace std;
using namespace QB;

typedef shared_ptr<vector<Declaration>> Declarations;
namespace QE {
    using Value = variant<string, int, vector<int>, vector<string>>;

    class DataPreprocessor {
        Table filerTableByDesignEntity(const Table& table, int colIdx, DesignEntity designEntity);
        Table filterTableByColValueEquality(const Table& table, const vector<int>& comparedCols);
        Table filerTableByColumnIdx(const Table& table, int colIdx, const string& value);
        DesignEntity getDesignEntityOfSyn(Synonym synonym);
        std::vector<std::string> intersection(std::vector<std::string> v1,
                                              std::vector<std::string> v2);
        std::vector<int> intersection(std::vector<int> v1,
                                              std::vector<int> v2);
        void getWithValues(vector<WithRef> withRefs, shared_ptr<vector<Value>> values);
        Table dropUnusedColumns(Table table);

        shared_ptr<DataRetriever> dataRetriever;
        Declarations declarations;

    public:
        explicit DataPreprocessor(shared_ptr<DataRetriever> dataRetriever, Declarations declarations);
        Table getAllByDesignEntity(DesignEntity designEntity);
        Table getNoConditionSelectClauseResult(shared_ptr<SelectClause> selectClause);
        Table getTableByAffects(shared_ptr<AffectsClause> affectsClause);
        Table getTableByAffectsT(shared_ptr<AffectsTClause> affectsTClause);
        Table getTableByCalls(shared_ptr<CallsClause> callsClause);
        Table getTableByCallsT(shared_ptr<CallsTClause> callsTClause);
        Table getTableByFollows(shared_ptr<FollowsClause> followsClause);
        Table getTableByFollowsT(shared_ptr<FollowsTClause> followsTClause);
        Table getTableByModifiesP(shared_ptr<ModifiesPClause> modifiesPClause);
        Table getTableByModifiesS(shared_ptr<ModifiesSClause> modifiesSClause);
        Table getTableByNext(shared_ptr<NextClause> nextClause);
        Table getTableByNextT(shared_ptr<NextTClause> nextTClause);
        Table getTableByParent(shared_ptr<ParentClause> parentClause);
        Table getTableByParentT(shared_ptr<ParentTClause> parentTClause);
        Table getTableByUsesP(shared_ptr<UsesPClause> usesPClause);
        Table getTableByUsesS(shared_ptr<UsesSClause> usesSClause);
        Table getTableByWith(shared_ptr<WithClause> withClause);
        Table getTableByAssignPattern(shared_ptr<QB::AssignPatternClause> assignPatternClause);
        Table getIfPatternTable(shared_ptr<QB::IfPatternClause> ifPatternClause);
        Table getWhilePatternTable(shared_ptr<WhilePatternClause> whilePatternClause);
        Table getCallsProcedureTable();
        Table getReadVariableTable();
        Table getPrintVariableTable();

        Table filterSingleClauseResultTable(Ref ref1, Ref ref2, Table table);
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

        bool hasResult(shared_ptr<WithClause> withClause);
    };

} // QE

#endif //SPA_DATAPREPROCESSOR_H
