//
// Created by Nafour on 31/8/22.
//
#include "catch.hpp"

#include "query_evaluator/QueryEvaluator.h"
#include "query_builder/QueryBuilder.h"
#include "Dummies/DummyQueryBuilder.h"
#include "query_evaluator/DataPreprocessor.h"
#include "DummyDataRetrievers/DummyDataRetriever.h"
#include <memory>

using namespace std;
using namespace QB;
using QE::QueryEvaluator;
using namespace std;
using namespace TestQE;

TEST_CASE("Test query evaluator") {
    shared_ptr<DummyDataRetriever> dummyDataRetriever = make_shared<DummyDataRetriever>();
    auto  dataPreprocessor= make_shared<QE::DataPreprocessor>(QE::DataPreprocessor(dummyDataRetriever));
    auto queryEvaluator = QueryEvaluator(dataPreprocessor);
    string queryStr = "variable v1; Select v1";

    auto query = (new TestQE::DummyQueryBuilder())
            ->addDeclaration(Declaration(QB::DesignEntity::VARIABLE, Synonym("v1")))
            ->addSelectClause(make_shared<SelectClause>(QB::SelectClause(Synonym("v1"))))
            ->buildPQLQuery();

    //auto query = QueryBuilder().buildPQLQuery(queryStr);
    string queryStr2 = "stmt a, b, c; Select a";
    auto query2 = QueryBuilder().buildPQLQuery(queryStr2);

    SECTION("evaluate variable v1; Select v1") {
        vector<string> header{"variable name"};
        vector<string> variables = vector<string>{"dummyVarA", "dummyVarB"};
        vector<vector<string>> rows;
        for (const string& variable: variables) {
            rows.push_back(vector<string>{variable});
        }
        Table variableTable;
        variableTable.rows = rows;
        variableTable.header = header;

        QE::QueryResult expected = QE::QueryResult(variableTable);
        QE::QueryResult queryResult = queryEvaluator.evaluate(query);

        REQUIRE(expected.table.rows.size() == queryResult.table.rows.size());

        auto expectedIt = expected.table.rows.begin();
        auto qRIt = queryResult.table.rows.begin();
        while (expectedIt != expected.table.rows.end()) {
            REQUIRE((*expectedIt)[0] == (*qRIt)[0]);
            qRIt++;
            expectedIt++;
        }
    }
}