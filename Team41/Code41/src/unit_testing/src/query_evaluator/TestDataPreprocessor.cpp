//
// Created by Nafour on 5/9/22.
//
#include "catch.hpp"
#include "query_builder/clauses/SuchThatClause.h"
#include "query_evaluator/DataPreprocessor.h"
#include "DummyDataRetrievers/DummyDataRetriever.h"
#include "DummyDataRetrievers/DummyDataRetriever2.h"
#include "Dummies/TableBuilder.h"
#include "query_evaluator/QEUtils.h"
#include "Dummies/DummyQueries.h"
TEST_CASE("Test Data Preprocessor") {
    //todo: replace fakeDR withh dummyDR
    shared_ptr<DummyDataRetriever> dummyDataRetriever = make_shared<DummyDataRetriever>();
    shared_ptr<QE::DataPreprocessor> dataPreprocessor = make_shared<QE::DataPreprocessor>(QE::DataPreprocessor(dummyDataRetriever));
/*
 * procedure p1 {
 * 1 dummyVarA = 1;
 * 2 dummyVarB = dummyVarA;
 * 3 print dummyVarA
 * }
 */
    SECTION("Test dummy retriever 1: assign a1, a2, get table such that follows(a1, a2)") {

        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("a1");
        Synonym syn2 = Synonym("a2");
        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn1)});
        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn2)});
        SuchThatClause suchThatClause = SuchThatClause(QB::RelationType::FOLLOWS, syn1, syn2, declarations);
        Table actual = dataPreprocessor->getTableByRelation(suchThatClause);

        Table expected = TestQE::TableBuilder().setHeaders({"a1", "a2"})
                ->addRow({"1", "2"})
                ->build();
        REQUIRE(actual.isEqual(expected) == true);
    }
    SECTION("Test dummy retriever 1: assign a; print p, get table such that follows(a, p)") {
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("a");
        Synonym syn2 = Synonym("p");
        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn1)});
        declarations->push_back({Declaration(QB::DesignEntity::PRINT, syn2)});
        SuchThatClause suchThatClause = SuchThatClause(QB::RelationType::FOLLOWS, syn1, syn2, declarations);
        Table actual = dataPreprocessor->getTableByRelation(suchThatClause);

        Table expected = TestQE::TableBuilder().setHeaders({"a", "p"})
                ->addRow({"2", "3"})
                ->build();
        REQUIRE(actual.isEqual(expected) == true);
    }
    SECTION("Test dummy retriever 1: assign a, a2, get table such that followsT(_, a2)") {
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn2 = Synonym("a2");
        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn2)});
        SuchThatClause suchThatClause = SuchThatClause(QB::RelationType::FOLLOWS_T, Underscore(), syn2, declarations);
        Table actual = dataPreprocessor->getTableByRelation(suchThatClause);

        Table expected = TestQE::TableBuilder().setHeaders({QE::QEUtils::getColNameByRefType(QB::RefType::UNDERSCORE), "a2"})
                ->addRow({"1","2"})
                ->build();
        REQUIRE(actual.isEqual(expected) == true);
    }
    SECTION("Test dummy retriever 1: assign a; print p, get table such that followsT(a, p)") {
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("a");
        Synonym syn2 = Synonym("p");
        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn1)});
        declarations->push_back({Declaration(QB::DesignEntity::PRINT, syn2)});
        SuchThatClause suchThatClause = SuchThatClause(QB::RelationType::FOLLOWS_T, syn1, syn2, declarations);
        Table actual = dataPreprocessor->getTableByRelation(suchThatClause);

        Table expected = TestQE::TableBuilder().setHeaders({"a", "p"})
                ->addRow({"2", "3"})
                ->addRow({"1", "3"})
                ->build();
        REQUIRE(actual.isEqual(expected) == true);
    }
    SECTION("Test dummy retriever 1: assign a;  get table such that followsT(a, _)") {
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("a");
        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn1)});
        SuchThatClause suchThatClause = SuchThatClause(QB::RelationType::FOLLOWS_T, syn1, Underscore(), declarations);
        Table actual = dataPreprocessor->getTableByRelation(suchThatClause);

        Table expected = TestQE::TableBuilder().setHeaders({"a", QE::QEUtils::getColNameByRefType(RefType::UNDERSCORE)})
                ->addRow({"1", "2"})
                ->addRow({"2", "3"})
                ->addRow({"1", "3"})
                ->build();
        REQUIRE(actual.isEqual(expected) == true);
    }
    SECTION("Test dummy retriever 1: assign a;  get table such that followsT(a, 3)") {
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("a");
        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn1)});
        SuchThatClause suchThatClause = SuchThatClause(QB::RelationType::FOLLOWS_T, syn1, 3, declarations);
        Table actual = dataPreprocessor->getTableByRelation(suchThatClause);

        Table expected = TestQE::TableBuilder().setHeaders({"a", QE::QEUtils::getColNameByRefType(RefType::INTEGER)})
                ->addRow({"2", "3"})
                ->addRow({"1", "3"})
                ->build();
        REQUIRE(actual.isEqual(expected) == true);
    }
    SECTION("Test dummy retriever 1: assign a, a2, get table such that followsT(1, a2)") {
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn2 = Synonym("a2");
        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn2)});
        SuchThatClause suchThatClause = SuchThatClause(QB::RelationType::FOLLOWS_T, 1, syn2, declarations);
        Table actual = dataPreprocessor->getTableByRelation(suchThatClause);

        Table expected = TestQE::TableBuilder().setHeaders({QE::QEUtils::getColNameByRefType(QB::RefType::INTEGER), "a2"})
                ->addRow({"1","2"})
                ->build();
        REQUIRE(actual.isEqual(expected) == true);
    }
     SECTION("Test dummy retriever 1: assign a; variable v, get table such that Modifies(a, v)") {
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("a");
        Synonym syn2 = Synonym("v");
        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn1)});
        declarations->push_back({Declaration(QB::DesignEntity::VARIABLE, syn2)});
        SuchThatClause suchThatClause = SuchThatClause(QB::RelationType::MODIFIES_S, syn1, syn2, declarations);
        Table actual = dataPreprocessor->getTableByRelation(suchThatClause);

        Table expected = TestQE::TableBuilder().setHeaders({"a", "v"})
                ->addRow({"1","dummyVarA"})
                ->addRow({"2","dummyVarB"})
                ->build();
        REQUIRE(actual.isEqual(expected) == true);
    }
    SECTION("Test dummy retriever 1: assign a;  get table such that Modifies(a, 'dummyVarA')") {
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("a");
        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn1)});
        SuchThatClause suchThatClause = SuchThatClause(QB::RelationType::MODIFIES_S, syn1, Ident("dummyVarA"), declarations);
        Table actual = dataPreprocessor->getTableByRelation(suchThatClause);

        Table expected = TestQE::TableBuilder().setHeaders({"a", QE::QEUtils::getColNameByRefType(QB::RefType::IDENT)})
                ->addRow({"1","dummyVarA"})
                ->build();
        REQUIRE(actual.isEqual(expected) == true);
    }
    SECTION("Test dummy retriever 1: assign a; variable v, get table such that Modifies(2, _)") {
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        SuchThatClause suchThatClause = SuchThatClause(QB::RelationType::MODIFIES_S, 2, Underscore(), declarations);
        Table actual = dataPreprocessor->getTableByRelation(suchThatClause);

        Table expected = TestQE::TableBuilder().setHeaders({QE::QEUtils::getColNameByRefType(QB::RefType::INTEGER),
                                                            QE::QEUtils::getColNameByRefType(QB::RefType::UNDERSCORE)})
                ->addRow({"2","dummyVarB"})
                ->build();
        REQUIRE(actual.isEqual(expected) == true);
    }

    shared_ptr<DummyDataRetriever2> dummyDataRetriever2 = make_shared<DummyDataRetriever2>();
    shared_ptr<QE::DataPreprocessor> dataPreprocessor2 = make_shared<QE::DataPreprocessor>(QE::DataPreprocessor(dummyDataRetriever2));

    SECTION("Test dummy retriever 2: assign a1, a2, get table such that follows(a1, a2)") {
        auto dummySuchThat = TestQE::DummyQueries::getDummySuchThat(0);
        Table actual = dataPreprocessor2->getTableByRelation(dummySuchThat);
        REQUIRE(actual.isBodyEmpty());
    }
    SECTION("Test dummy retriever 2: while w, get table such that followsT(1, w)") {
        auto dummySuchThat = TestQE::DummyQueries::getDummySuchThat(1);
        Table actual = dataPreprocessor2->getTableByRelation(dummySuchThat);
        Table expected = FollowsTable();
        expected.renameHeader({QE::QEUtils::getColNameByRefType(QB::RefType::INTEGER), "w"});
        expected.appendRow({"1", "3"});
        REQUIRE(actual.isEqual(expected));
    }
    SECTION("Test dummy retriever 2: assign a, get table such that Parent(_, a)") {
        auto dummySuchThat = TestQE::DummyQueries::getDummySuchThat(2);
        Table actual = dataPreprocessor2->getTableByRelation(dummySuchThat);
        Table expected = FollowsTable();
        expected.renameHeader({QE::QEUtils::getColNameByRefType(QB::RefType::UNDERSCORE), "a"});
        expected.appendRow({"3", "4"});
        expected.appendRow({"3", "7"});
        expected.appendRow({"5", "6"});
        REQUIRE(actual.isEqual(expected));
    }
    SECTION("Test dummy retriever 2: assign a, get table such that Uses(a, 'y')") {
        auto dummySuchThat = TestQE::DummyQueries::getDummySuchThat(3);
        Table actual = dataPreprocessor2->getTableByRelation(dummySuchThat);
        Table expected = UsesTable();
        expected.renameHeader({"a", QE::QEUtils::getColNameByRefType(QB::RefType::IDENT)});
        expected.appendRow({"7", "y"});
        REQUIRE(actual.isEqual(expected));
    }
}