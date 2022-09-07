//
// Created by Nafour on 5/9/22.
//
#include "catch.hpp"
#include "query_builder/clauses/SuchThatClause.h"
#include "query_evaluator/DataPreprocessor.h"
#include "DummyDataRetrievers/DummyDataRetriever.h"
#include "Dummies/TableBuilder.h"

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

        Table expected = TestQE::TableBuilder().setHeaders({QE::getColNameByRefType(QB::RefType::UNDERSCORE), "a2"})
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

        Table expected = TestQE::TableBuilder().setHeaders({"a", QE::getColNameByRefType(RefType::UNDERSCORE)})
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

        Table expected = TestQE::TableBuilder().setHeaders({"a", QE::getColNameByRefType(RefType::INTEGER)})
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

        Table expected = TestQE::TableBuilder().setHeaders({QE::getColNameByRefType(QB::RefType::INTEGER), "a2"})
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

        Table expected = TestQE::TableBuilder().setHeaders({"a", QE::getColNameByRefType(QB::RefType::IDENT)})
                ->addRow({"1","dummyVarA"})
                ->build();
        REQUIRE(actual.isEqual(expected) == true);
    }
    SECTION("Test dummy retriever 1: assign a; variable v, get table such that Modifies(2, _)") {
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        SuchThatClause suchThatClause = SuchThatClause(QB::RelationType::MODIFIES_S, 2, Underscore(), declarations);
        Table actual = dataPreprocessor->getTableByRelation(suchThatClause);

        Table expected = TestQE::TableBuilder().setHeaders({QE::getColNameByRefType(QB::RefType::INTEGER),
                                                            QE::getColNameByRefType(QB::RefType::UNDERSCORE)})
                ->addRow({"2","dummyVarB"})
                ->build();
        REQUIRE(actual.isEqual(expected) == true);
    }
}