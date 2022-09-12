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
#include "Dummies/TableBuilder.h"
#include "query_evaluator/QEUtils.h"
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

    SECTION("dummy retriever 1: evaluate variable v1; Select v1") {
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
    SECTION("dummy retriever 1: evaluate variable v1; Select v1 such that Modifies(1, v1)") {
        Synonym syn1 = Synonym("v1");
        auto declarations = make_shared<vector<Declaration>>(vector<Declaration>{Declaration(QB::DesignEntity::VARIABLE, syn1)});
        auto query3 = DummyQueryBuilder()
                .addDeclarations(declarations)
                ->addSelectClause(make_shared<SelectClause>(SelectClause(syn1)))
                ->addSuchThatClause(make_shared<SuchThatClause>(SuchThatClause(QB::RelationType::MODIFIES_S, 1, syn1, declarations)))
                ->buildPQLQuery();
        QE::QueryResult actual = queryEvaluator.evaluate(query3);
        QE::QueryResult expected = QE::QueryResult();
        expected.colName = "v1";
        expected.table = TableBuilder().setHeaders({QE::QEUtils::getColNameByRefType(QB::RefType::INTEGER), "v1"})
                ->addRow({"1", "dummyVarA"})
                ->build();
        REQUIRE(actual.isEqual(expected) == true);
    }
    SECTION("dummy retriever 1: evaluate variable v1; Select v1 such that Modifies(_, v1)") {
        Synonym syn1 = Synonym("v1");
        auto declarations = make_shared<vector<Declaration>>(vector<Declaration>{Declaration(QB::DesignEntity::VARIABLE, syn1)});
        auto query3 = DummyQueryBuilder()
                .addDeclarations(declarations)
                ->addSelectClause(make_shared<SelectClause>(SelectClause(syn1)))
                ->addSuchThatClause(make_shared<SuchThatClause>(SuchThatClause(QB::RelationType::MODIFIES_S, Underscore(), syn1, declarations)))
                ->buildPQLQuery();
        QE::QueryResult actual = queryEvaluator.evaluate(query3);
        QE::QueryResult expected = QE::QueryResult();
        expected.colName = "v1";
        expected.table = TableBuilder().setHeaders({QE::QEUtils::getColNameByRefType(QB::RefType::UNDERSCORE), "v1"})
                ->addRow({"1", "dummyVarA"})
                ->addRow({"2", "dummyVarB"})
                ->build();
        REQUIRE(actual.isEqual(expected) == true);
    }
    SECTION("dummy retriever 1: evaluate assgin a ; Select a such that Modifies(a, 'dummyVarA')") {
        Synonym syn1 = Synonym("a");
        auto declarations = make_shared<vector<Declaration>>(vector<Declaration>{Declaration(QB::DesignEntity::ASSIGN, syn1)});
        auto query3 = DummyQueryBuilder()
                .addDeclarations(declarations)
                ->addSelectClause(make_shared<SelectClause>(SelectClause(syn1)))
                ->addSuchThatClause(make_shared<SuchThatClause>(SuchThatClause(QB::RelationType::MODIFIES_S, syn1, Ident("dummyVarA"), declarations)))
                ->buildPQLQuery();
        QE::QueryResult actual = queryEvaluator.evaluate(query3);
        QE::QueryResult expected = QE::QueryResult();
        expected.colName = "a";
        expected.table = TableBuilder().setHeaders({"a",
                                                    QE::QEUtils::getColNameByRefType(QB::RefType::IDENT)})
                ->addRow({"1", "dummyVarA"})
                ->build();
        REQUIRE(actual.isEqual(expected) == true);
    }
    SECTION("dummy retriever 1: evaluate assgin a ; Select a such that Follows(a, _)") {
        Synonym syn1 = Synonym("a");
        auto declarations = make_shared<vector<Declaration>>(vector<Declaration>{Declaration(QB::DesignEntity::ASSIGN, syn1)});
        auto query3 = DummyQueryBuilder()
                .addDeclarations(declarations)
                ->addSelectClause(make_shared<SelectClause>(SelectClause(syn1)))
                ->addSuchThatClause(make_shared<SuchThatClause>(SuchThatClause(QB::RelationType::FOLLOWS, syn1, Underscore(), declarations)))
                ->buildPQLQuery();
        QE::QueryResult actual = queryEvaluator.evaluate(query3);
        QE::QueryResult expected = QE::QueryResult();
        expected.colName = "a";
        expected.table = TableBuilder().setHeaders({"a",
                                                    QE::QEUtils::getColNameByRefType(QB::RefType::UNDERSCORE)})
                ->addRow({"1", "2"})
                ->addRow({"2", "3"})
                ->build();
        REQUIRE(actual.isEqual(expected) == true);
    }
    SECTION("dummy retriever 1: evaluate assgin a ; Select a such that Follows(a, 3)") {
        Synonym syn1 = Synonym("a");
        auto declarations = make_shared<vector<Declaration>>(vector<Declaration>{Declaration(QB::DesignEntity::ASSIGN, syn1)});
        auto query3 = DummyQueryBuilder()
                .addDeclarations(declarations)
                ->addSelectClause(make_shared<SelectClause>(SelectClause(syn1)))
                ->addSuchThatClause(make_shared<SuchThatClause>(SuchThatClause(QB::RelationType::FOLLOWS, syn1, 3, declarations)))
                ->buildPQLQuery();
        QE::QueryResult actual = queryEvaluator.evaluate(query3);
        QE::QueryResult expected = QE::QueryResult();
        expected.colName = "a";
        expected.table = TableBuilder().setHeaders({"a",
                                                    QE::QEUtils::getColNameByRefType(QB::RefType::INTEGER)})

                ->addRow({"2", "3"})
                ->build();
        REQUIRE(actual.isEqual(expected) == true);
    }
}