//
// Created by Nafour on 5/9/22.
//
#include "catch.hpp"
#include "query_builder/clauses/such_that_clauses/SuchThatClause.h"
#include "query_evaluator/DataPreprocessor.h"
#include "DummyDataRetrievers/DummyDataRetriever.h"
#include "DummyDataRetrievers/DummyDataRetriever2.h"
#include "Dummies/TableBuilder.h"
#include "query_evaluator/QEUtils.h"
#include "query_builder/clauses/such_that_clauses/FollowsClause.h"
#include "query_builder/clauses/such_that_clauses/FollowsTClause.h"
#include "query_builder/clauses/such_that_clauses/ModifiesSClause.h"
#include "query_builder/clauses/such_that_clauses/ParentClause.h"
#include "query_builder/clauses/such_that_clauses/UsesSClause.h"

TEST_CASE("Test Data Preprocessor") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DummyDataRetriever> dummyDataRetriever = make_shared<DummyDataRetriever>(pkbStorage);
/*
 * procedure p1 {
 * 1 dummyVarA = 1;
 * 2 dummyVarB = dummyVarA;
 * 3 print dummyVarA
 * }
 */
    SECTION("Test dummy retriever 1: assign a1, a2, get table such that follows(a1, a2)") {
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        shared_ptr<QE::DataPreprocessor> dataPreprocessor = make_shared<QE::DataPreprocessor>(dummyDataRetriever, declarations);
        Synonym syn1 = Synonym("a1");
        Synonym syn2 = Synonym("a2");
        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn1)});
        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn2)});

        auto followsCl = make_shared<FollowsClause>(syn1, syn2);
        Table actual = dataPreprocessor->getTableByFollows(followsCl);

        Table expected = TestQE::TableBuilder().setHeaders({"a1", "a2"})
                ->addRow({"1", "2"})
                ->build();
        REQUIRE(actual.isEqual(expected) == true);
    }
    SECTION("Test dummy retriever 1: assign a; print p, get table such that follows(a, p)") {

        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        shared_ptr<QE::DataPreprocessor> dataPreprocessor = make_shared<QE::DataPreprocessor>(dummyDataRetriever, declarations);
        Synonym syn1 = Synonym("a");
        Synonym syn2 = Synonym("p");
        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn1)});
        declarations->push_back({Declaration(QB::DesignEntity::PRINT, syn2)});
        Table actual = dataPreprocessor->getTableByFollows(make_shared<FollowsClause>(syn1, syn2));

        Table expected = TestQE::TableBuilder().setHeaders({"a", "p"})
                ->addRow({"2", "3"})
                ->build();
        REQUIRE(actual.isEqual(expected) == true);
    }
    SECTION("Test dummy retriever 1: assign a, a2, get table such that followsT(_, a2)") {
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        shared_ptr<QE::DataPreprocessor> dataPreprocessor = make_shared<QE::DataPreprocessor>(dummyDataRetriever, declarations);
        Synonym syn2 = Synonym("a2");
        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn2)});
        Table actual = dataPreprocessor->getTableByFollowsT(make_shared<FollowsTClause>(Underscore(), syn2));

        Table expected = TestQE::TableBuilder().setHeaders({ "a2"})
                ->addRow({"2"})
                ->build();
        REQUIRE(actual.isEqual(expected) == true);
    }
    SECTION("Test dummy retriever 1: assign a; print p, get table such that followsT(a, p)") {
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        shared_ptr<QE::DataPreprocessor> dataPreprocessor = make_shared<QE::DataPreprocessor>(dummyDataRetriever, declarations);
        Synonym syn1 = Synonym("a");
        Synonym syn2 = Synonym("p");
        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn1)});
        declarations->push_back({Declaration(QB::DesignEntity::PRINT, syn2)});
        Table actual = dataPreprocessor->getTableByFollowsT(make_shared<FollowsTClause>(syn1, syn2));

        Table expected = TestQE::TableBuilder().setHeaders({"a", "p"})
                ->addRow({"2", "3"})
                ->addRow({"1", "3"})
                ->build();
        REQUIRE(actual.isEqual(expected) == true);
    }
    SECTION("Test dummy retriever 1: assign a;  get table such that followsT(a, _)") {
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        shared_ptr<QE::DataPreprocessor> dataPreprocessor = make_shared<QE::DataPreprocessor>(dummyDataRetriever, declarations);
        Synonym syn1 = Synonym("a");
        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn1)});
        Table actual = dataPreprocessor->getTableByFollowsT(make_shared<FollowsTClause>(syn1, Underscore()));

        Table expected = TestQE::TableBuilder().setHeaders({"a"})
                ->addRow({"1"})
                ->addRow({"2"})
                ->addRow({"1"})
                ->build();
        REQUIRE(actual.isEqual(expected) == true);
    }
    SECTION("Test dummy retriever 1: assign a;  get table such that followsT(a, 3)") {
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        shared_ptr<QE::DataPreprocessor> dataPreprocessor = make_shared<QE::DataPreprocessor>(dummyDataRetriever, declarations);
        Synonym syn1 = Synonym("a");
        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn1)});
        Table actual = dataPreprocessor->getTableByFollowsT(make_shared<FollowsTClause>(syn1, 3));

        Table expected = TestQE::TableBuilder().setHeaders({"a"})
                ->addRow({"2"})
                ->addRow({"1"})
                ->build();
        REQUIRE(actual.isEqual(expected) == true);
    }
    SECTION("Test dummy retriever 1: assign a, a2, get table such that followsT(1, a2)") {
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        shared_ptr<QE::DataPreprocessor> dataPreprocessor = make_shared<QE::DataPreprocessor>(dummyDataRetriever, declarations);
        Synonym syn2 = Synonym("a2");
        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn2)});
        Table actual = dataPreprocessor->getTableByFollowsT(make_shared<FollowsTClause>(1, syn2));

        Table expected = TestQE::TableBuilder().setHeaders({"a2"})
                ->addRow({"2"})
                ->build();
        REQUIRE(actual.isEqual(expected) == true);
    }
     SECTION("Test dummy retriever 1: assign a; variable v, get table such that Modifies(a, v)") {
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        shared_ptr<QE::DataPreprocessor> dataPreprocessor = make_shared<QE::DataPreprocessor>(dummyDataRetriever, declarations);
        Synonym syn1 = Synonym("a");
        Synonym syn2 = Synonym("v");
        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn1)});
        declarations->push_back({Declaration(QB::DesignEntity::VARIABLE, syn2)});
        Table actual = dataPreprocessor->getTableByModifiesS(make_shared<ModifiesSClause>(syn1, syn2));

        Table expected = TestQE::TableBuilder().setHeaders({"a", "v"})
                ->addRow({"1","dummyVarA"})
                ->addRow({"2","dummyVarB"})
                ->build();
        REQUIRE(actual.isEqual(expected) == true);
    }
    SECTION("Test dummy retriever 1: assign a;  get table such that Modifies(a, 'dummyVarA')") {
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        shared_ptr<QE::DataPreprocessor> dataPreprocessor = make_shared<QE::DataPreprocessor>(dummyDataRetriever, declarations);
        Synonym syn1 = Synonym("a");
        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn1)});
        Table actual = dataPreprocessor->getTableByModifiesS(make_shared<ModifiesSClause>(syn1, Ident("dummyVarA")));

        Table expected = TestQE::TableBuilder().setHeaders({"a"})
                ->addRow({"1"})
                ->build();
        REQUIRE(actual.isEqual(expected) == true);
    }
    SECTION("Test dummy retriever 1: assign a; variable v, get table such that Modifies(2, _)") {
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        shared_ptr<QE::DataPreprocessor> dataPreprocessor = make_shared<QE::DataPreprocessor>(dummyDataRetriever, declarations);
        Table actual = dataPreprocessor->getTableByModifiesS(make_shared<ModifiesSClause>(2, Underscore()));

        const string DUMMY_HEADER = "$dummy_header";
        const string DUMMY_VALUE = "$dummy_value";
        Table dummyTable = Table();
        dummyTable.renameHeader({DUMMY_HEADER}) ;
        dummyTable.rows = vector<vector<string>>({{DUMMY_VALUE}});

        Table expected = TestQE::TableBuilder().setHeaders({QE::QEUtils::getColNameByRefType(QB::RefType::INTEGER),
                                                            QE::QEUtils::getColNameByRefType(QB::RefType::UNDERSCORE)})
                ->addRow({"2","dummyVarB"})
                ->build();
        REQUIRE(actual.isEqual(dummyTable));
    }
    shared_ptr<PKBStorage> pkbStorage2 = make_shared<PKBStorage>();
    shared_ptr<DummyDataRetriever2> dummyDataRetriever2 = make_shared<DummyDataRetriever2>(pkbStorage2);



    SECTION("Test dummy retriever 2: assign a1, a2, get table such that follows(a1, a2)") {
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("a1");
        Synonym syn2 = Synonym("a2");
        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn1)});
        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn2)});
        shared_ptr<QE::DataPreprocessor> dataPreprocessor2 = make_shared<QE::DataPreprocessor>(dummyDataRetriever2, declarations);
        Table actual = dataPreprocessor2->getTableByFollows(make_shared<FollowsClause>(syn1, syn2));
        REQUIRE(actual.isBodyEmpty());
    }
    SECTION("Test dummy retriever 2: while w, get table such that followsT(1, w)") {
        shared_ptr<vector<Declaration>> declarations_2 = make_shared<vector<Declaration>>();
        Synonym syn2_2 = Synonym("w");
        declarations_2->push_back({Declaration(QB::DesignEntity::WHILE, syn2_2)});

        shared_ptr<QE::DataPreprocessor> dataPreprocessor2 = make_shared<QE::DataPreprocessor>(dummyDataRetriever2, declarations_2);

        Table actual = dataPreprocessor2->getTableByFollowsT(make_shared<FollowsTClause>(1, syn2_2));
        Table expected = FollowsTable();
        expected.renameHeader({ "w"});
        expected.appendRow({ "3"});
        REQUIRE(actual.isEqual(expected));
    }
    SECTION("Test dummy retriever 2: assign a, get table such that Parent(_, a)") {
        shared_ptr<vector<Declaration>> declarations_3 = make_shared<vector<Declaration>>();
        Synonym syn2_3 = Synonym("a");
        declarations_3->push_back({Declaration(QB::DesignEntity::ASSIGN, syn2_3)});

        shared_ptr<QE::DataPreprocessor> dataPreprocessor2 = make_shared<QE::DataPreprocessor>(dummyDataRetriever2, declarations_3);

        Table actual = dataPreprocessor2->getTableByParent(make_shared<ParentClause>(Underscore(), syn2_3));
        Table expected = FollowsTable();
        expected.renameHeader({"a"});
        expected.appendRow({ "4"});
        expected.appendRow({ "7"});
        expected.appendRow({ "6"});
        REQUIRE(actual.isEqual(expected));
    }
    SECTION("Test dummy retriever 2: assign a, get table such that Uses(a, 'y')") {
        shared_ptr<vector<Declaration>> declarations_4 = make_shared<vector<Declaration>>();
        Synonym syn1_4 = Synonym("a");
        declarations_4->push_back({Declaration(QB::DesignEntity::ASSIGN, syn1_4)});
        shared_ptr<QE::DataPreprocessor> dataPreprocessor2 = make_shared<QE::DataPreprocessor>(dummyDataRetriever2, declarations_4);
        Table actual = dataPreprocessor2->getTableByUsesS(make_shared<UsesSClause>(syn1_4, Ident("y")));
        Table expected = UsesTable();
        expected.renameHeader({"a"});
        expected.appendRow({"7"});
        REQUIRE(actual.isEqual(expected));
    }
}