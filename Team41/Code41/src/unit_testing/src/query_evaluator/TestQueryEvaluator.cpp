//
// Created by Nafour on 21/9/22.
//
#include "catch.hpp"
#include <memory>
#include "query_builder/commons/Query.h"
#include "query_evaluator/QueryEvaluator.h"
#include "DummyDataRetrievers/DummyDataRetriever.h"
#include "DummyDataRetrievers/DummyDataRetriever2.h"
#include "QETestUtils.h"
#include "query_builder/clauses/pattern_clauses/AssignPatternClause.h"

using namespace std;
TEST_CASE("Test query evaluator") {
    const vector<string> FALSE_RESULT = {"FALSE"};
    const vector<string> TRUE_RESULT = {"TRUE"};
    const vector<string> EMPTY_RESULT = {};
    shared_ptr<DummyDataRetriever> dummyDataRetriever = make_shared<DummyDataRetriever>(make_shared<PKBStorage>());
    QE::QueryEvaluator queryEvaluator = QE::QueryEvaluator(dummyDataRetriever);
    SECTION("test no clause select bool") {
        shared_ptr<Query> query = make_shared<Query>();
        query->selectClause = make_shared<SelectClause>(ReturnType::BOOLEAN);


        auto actual = queryEvaluator.evaluate(query);
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, TRUE_RESULT));
    }
    SECTION("test select single tuple: variable v; select v") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("v");
        declarations->push_back(Declaration(QB::DesignEntity::VARIABLE, syn1));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(syn1);

        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);

        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"dummyVarA", "dummyVarB"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select single tuple: stmt s; select s") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("s");
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn1));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(syn1);

        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);


        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"1", "2", "3"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select single tuple: assign a; select a") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("a");
        declarations->push_back(Declaration(QB::DesignEntity::ASSIGN, syn1));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(syn1);

        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);

        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"1", "2"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select single tuple with empty result: read r; select r") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("r");
        declarations->push_back(Declaration(QB::DesignEntity::READ, syn1));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(syn1);

        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);

        auto actual = queryEvaluator.evaluate(query);
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, EMPTY_RESULT));
    }
    SECTION("test select tuple with two elements: variable v; stmt s; select <v, s>") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("v");
        Synonym syn2 = Synonym("s");

        declarations->push_back(Declaration(QB::DesignEntity::VARIABLE, syn1));
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn2));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(syn1);
        returnResults->push_back(syn2);

        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);

        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"dummyVarA 1", "dummyVarB 1", "dummyVarA 2", "dummyVarB 2", "dummyVarA 3", "dummyVarB 3"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select tuple with two elements and with empty result: variable v; read r; select <v, r>") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("v");
        Synonym syn2 = Synonym("r");

        declarations->push_back(Declaration(QB::DesignEntity::VARIABLE, syn1));
        declarations->push_back(Declaration(QB::DesignEntity::READ, syn2));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(syn1);
        returnResults->push_back(syn2);

        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);

        auto actual = queryEvaluator.evaluate(query);
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, EMPTY_RESULT));
    }
    SECTION("test select single tuple: variable v; select v such that modifies(1, v)") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("v");
        declarations->push_back(Declaration(QB::DesignEntity::VARIABLE, syn1));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(syn1);

        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);
        shared_ptr<SuchThatClause> suchThatClause1 =
                make_shared<SuchThatClause>(RelationType::MODIFIES_S, 1, syn1, declarations);
        query->suchThatClauses->push_back(suchThatClause1);
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"dummyVarA"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select single tuple: stmt s; select s such that modifies(s, _)") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("s");
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn1));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(syn1);

        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);
        shared_ptr<SuchThatClause> suchThatClause1 =
                make_shared<SuchThatClause>(RelationType::MODIFIES_S, syn1, Underscore(), declarations);
        query->suchThatClauses->push_back(suchThatClause1);
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"1", "2"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select single tuple: stmt s; select s such that follows*(_, 3)") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("s");
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn1));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(syn1);

        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);
        shared_ptr<SuchThatClause> suchThatClause1 =
                make_shared<SuchThatClause>(RelationType::FOLLOWS_T, Underscore(), 3, declarations);
        query->suchThatClauses->push_back(suchThatClause1);
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"1", "2", "3"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return true: stmt s; select bool such that follows(_, 3)") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("s");
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn1));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(syn1);

        query->selectClause = make_shared<SelectClause>(ReturnType::BOOLEAN);
        shared_ptr<SuchThatClause> suchThatClause1 =
                make_shared<SuchThatClause>(RelationType::FOLLOWS, Underscore(), 3, declarations);
        query->suchThatClauses->push_back(suchThatClause1);
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = TRUE_RESULT;
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return false: assign a; select bool such that uses(a, 'dummyVarB')") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("a");
        declarations->push_back(Declaration(QB::DesignEntity::ASSIGN, syn1));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(syn1);

        query->selectClause = make_shared<SelectClause>(ReturnType::BOOLEAN);
        shared_ptr<SuchThatClause> suchThatClause1 =
                make_shared<SuchThatClause>(RelationType::FOLLOWS_T, Underscore(), 1, declarations);
        query->suchThatClauses->push_back(suchThatClause1);
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = FALSE_RESULT;
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select single tuple with two clauses: stmt s1, s2, s3; select s2 such that follows(s1, s2) and follows(s2, s3)") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("s1");
        Synonym syn2 = Synonym("s2");
        Synonym syn3 = Synonym("s3");
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn1));
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn2));
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn3));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(syn2);

        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);
        shared_ptr<SuchThatClause> suchThatClause1 =
                make_shared<SuchThatClause>(RelationType::FOLLOWS, syn1, syn2, declarations);
        shared_ptr<SuchThatClause> suchThatClause2 =
                make_shared<SuchThatClause>(RelationType::FOLLOWS, syn2, syn3, declarations);
        query->suchThatClauses->push_back(suchThatClause1);
        query->suchThatClauses->push_back(suchThatClause2);
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"2"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select two tuple with two clauses: stmt s1, s2, s3; select <s1,s2> such that follows(s1, s2) and follows(s2, s3)") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("s1");
        Synonym syn2 = Synonym("s2");
        Synonym syn3 = Synonym("s3");
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn1));
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn2));
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn3));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();

        *returnResults = {syn1, syn2};
        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);
        shared_ptr<SuchThatClause> suchThatClause1 =
                make_shared<SuchThatClause>(RelationType::FOLLOWS, syn1, syn2, declarations);
        shared_ptr<SuchThatClause> suchThatClause2 =
                make_shared<SuchThatClause>(RelationType::FOLLOWS, syn2, syn3, declarations);
        query->suchThatClauses->push_back(suchThatClause1);
        query->suchThatClauses->push_back(suchThatClause2);
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"1 2"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select three tuple with two clauses: stmt s1, s2; variable v; select <s1, s2, v> such that follows(s1, s2) and uses(s2, _)") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("s1");
        Synonym syn2 = Synonym("s2");
        Synonym syn3 = Synonym("v");
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn1));
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn2));
        declarations->push_back(Declaration(QB::DesignEntity::VARIABLE, syn3));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();

        *returnResults = {syn1, syn2, syn3};
        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);
        shared_ptr<SuchThatClause> suchThatClause1 =
                make_shared<SuchThatClause>(RelationType::FOLLOWS, syn1, syn2, declarations);
        shared_ptr<SuchThatClause> suchThatClause2 =
                make_shared<SuchThatClause>(RelationType::USES_S, syn2, Underscore(), declarations);
        query->suchThatClauses->push_back(suchThatClause1);
        query->suchThatClauses->push_back(suchThatClause2);
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"1 2 dummyVarA", "2 3 dummyVarA", "1 2 dummyVarB", "2 3 dummyVarB"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select four tuple with three clauses: stmt s1, s2; variable v; constant c; select <s1, c, s2, v> such that follows(s1, s2) and uses(s2, _) and modifies(s1, 'dummyVarB')") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("s1");
        Synonym syn2 = Synonym("s2");
        Synonym syn3 = Synonym("v");
        Synonym syn4 = Synonym("c");
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn1));
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn2));
        declarations->push_back(Declaration(QB::DesignEntity::VARIABLE, syn3));
        declarations->push_back(Declaration(QB::DesignEntity::CONSTANT, syn4));

        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();

        *returnResults = {syn1, syn4, syn2, syn3};
        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);
        shared_ptr<SuchThatClause> suchThatClause1 =
                make_shared<SuchThatClause>(RelationType::FOLLOWS, syn1, syn2, declarations);
        shared_ptr<SuchThatClause> suchThatClause2 =
                make_shared<SuchThatClause>(RelationType::USES_S, syn2, Underscore(), declarations);
        shared_ptr<SuchThatClause> suchThatClause3 =
                make_shared<SuchThatClause>(RelationType::MODIFIES_S, syn1, Ident("dummyVarB"), declarations);
        query->suchThatClauses->push_back(suchThatClause1);
        query->suchThatClauses->push_back(suchThatClause2);
        query->suchThatClauses->push_back(suchThatClause3);
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = { "2 1 3 dummyVarB", "2 1 3 dummyVarA"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select single with one pattern clause: assign a; select a pattern a('dummyVarA', _)") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("a");
        declarations->push_back(Declaration(QB::DesignEntity::ASSIGN, syn1));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();

        *returnResults = {syn1};
        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);

        ExpressionSpec expressionSpec = ExpressionSpec(QB::ExpressionSpecType::ANY_MATCH);
        shared_ptr<PatternClause> patternClause1 = make_shared<AssignPatternClause>( syn1, Ident("dummyVarA"), expressionSpec);
        query->patternClauses->push_back(patternClause1);
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"1"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select single tuple with one pattern clause and one such that: assign a; variable v; select a pattern a(v, _) such that follows(1, a)") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("a");
        Synonym syn2 = Synonym("v");
        declarations->push_back(Declaration(QB::DesignEntity::ASSIGN, syn1));
        declarations->push_back(Declaration(QB::DesignEntity::VARIABLE, syn2));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();

        *returnResults = {syn1};
        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);

        ExpressionSpec expressionSpec = ExpressionSpec(QB::ExpressionSpecType::ANY_MATCH);
        shared_ptr<PatternClause> patternClause1 = make_shared<AssignPatternClause>( syn1, syn2, expressionSpec);
        query->patternClauses->push_back(patternClause1);

        shared_ptr<SuchThatClause> suchThatClause1 =
                make_shared<SuchThatClause>(RelationType::FOLLOWS, 1, syn1, declarations);
        query->suchThatClauses->push_back(suchThatClause1);


        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"2"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select two tuple with one pattern clause and one such that: assign a; variable v; select <v, a> pattern a(v, _) such that follows(1, a)") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("a");
        Synonym syn2 = Synonym("v");
        declarations->push_back(Declaration(QB::DesignEntity::ASSIGN, syn1));
        declarations->push_back(Declaration(QB::DesignEntity::VARIABLE, syn2));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();

        *returnResults = {syn2, syn1};
        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);

        ExpressionSpec expressionSpec = ExpressionSpec(QB::ExpressionSpecType::ANY_MATCH);
        shared_ptr<PatternClause> patternClause1 = make_shared<AssignPatternClause>( syn1, syn2, expressionSpec);
        query->patternClauses->push_back(patternClause1);

        shared_ptr<SuchThatClause> suchThatClause1 =
                make_shared<SuchThatClause>(RelationType::FOLLOWS, 1, syn1, declarations);
        query->suchThatClauses->push_back(suchThatClause1);


        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"dummyVarB 2"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }

    shared_ptr<DummyDataRetriever2> dummyDataRetriever2 = make_shared<DummyDataRetriever2>(make_shared<PKBStorage>());
    QE::QueryEvaluator queryEvaluator2 = QE::QueryEvaluator(dummyDataRetriever2);
    SECTION("test no clause select bool") {
        shared_ptr<Query> query = make_shared<Query>();
        query->selectClause = make_shared<SelectClause>(ReturnType::BOOLEAN);


        auto actual = queryEvaluator2.evaluate(query);
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, TRUE_RESULT));
    }
    SECTION("test select single tuple: variable v; select v") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("v");
        declarations->push_back(Declaration(QB::DesignEntity::VARIABLE, syn1));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(syn1);

        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);

        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"dummyVarA", "dummyVarB"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select single tuple: variable v; select v") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("v");
        declarations->push_back(Declaration(QB::DesignEntity::VARIABLE, syn1));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(syn1);

        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);

        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"x", "y"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select single tuple: while s; select s") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("s");
        declarations->push_back(Declaration(QB::DesignEntity::WHILE, syn1));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(syn1);

        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);


        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"3"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select single tuple: if ifs; select a") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("ifs");
        declarations->push_back(Declaration(QB::DesignEntity::IF, syn1));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(syn1);

        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);

        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"5"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select single tuple with empty result: call c; select c") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("c");
        declarations->push_back(Declaration(QB::DesignEntity::CALL, syn1));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(syn1);

        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);

        auto actual = queryEvaluator2.evaluate(query);
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, EMPTY_RESULT));
    }
    SECTION("test select tuple with two elements: variable v; assign a; select <v, a>") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("v");
        Synonym syn2 = Synonym("a");

        declarations->push_back(Declaration(QB::DesignEntity::VARIABLE, syn1));
        declarations->push_back(Declaration(QB::DesignEntity::ASSIGN, syn2));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(syn1);
        returnResults->push_back(syn2);

        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);

        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"x 4", "x 6", "x 7","y 4", "y 6", "y 7"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select tuple with two elements: variable v; assign a; select <a, v>") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("v");
        Synonym syn2 = Synonym("a");

        declarations->push_back(Declaration(QB::DesignEntity::VARIABLE, syn1));
        declarations->push_back(Declaration(QB::DesignEntity::ASSIGN, syn2));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(syn2);
        returnResults->push_back(syn1);

        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);

        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"4 x", "6 x", "7 x","4 y", "6 y", "7 y"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select tuple with two elements: variable v; assign a; select <v, v>") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("v");
        Synonym syn2 = Synonym("a");

        declarations->push_back(Declaration(QB::DesignEntity::VARIABLE, syn1));
        declarations->push_back(Declaration(QB::DesignEntity::ASSIGN, syn2));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(syn1);
        returnResults->push_back(syn1);

        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);

        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"x x","y y", "x y", "y x"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select tuple with two elements and with empty result: variable v; call c; select <v, c>") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("v");
        Synonym syn2 = Synonym("c");

        declarations->push_back(Declaration(QB::DesignEntity::VARIABLE, syn1));
        declarations->push_back(Declaration(QB::DesignEntity::CALL, syn2));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(syn1);
        returnResults->push_back(syn2);

        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);

        auto actual = queryEvaluator.evaluate(query);
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, EMPTY_RESULT));
        //test select tuple with two elements and with empty result: variable v; call c; select <c, v>
        shared_ptr<vector<Elem>> returnResults2 = make_shared<vector<Elem>>();
        returnResults2->push_back(syn2);
        returnResults2->push_back(syn1);
        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults2);
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, EMPTY_RESULT));
    }
    SECTION("test select single tuple: stmt s; select s such that parent(_, s)") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("s");
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn1));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(syn1);

        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);
        shared_ptr<SuchThatClause> suchThatClause1 =
                make_shared<SuchThatClause>(RelationType::PARENT, Underscore(), syn1, declarations);
        query->suchThatClauses->push_back(suchThatClause1);
        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"4", "5","6", "7"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));

    }
    SECTION("test select single tuple: read r; select r such that uses(3, 'y')") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("r");
        declarations->push_back(Declaration(QB::DesignEntity::READ, syn1));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(syn1);

        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);
        shared_ptr<SuchThatClause> suchThatClause1 =
                make_shared<SuchThatClause>(RelationType::USES_S, 3, Ident("y"), declarations);
        query->suchThatClauses->push_back(suchThatClause1);
        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"1", "2"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return false: assign a; select bool such that parent(8, _)") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("a");
        declarations->push_back(Declaration(QB::DesignEntity::ASSIGN, syn1));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(syn1);

        query->selectClause = make_shared<SelectClause>(ReturnType::BOOLEAN);
        shared_ptr<SuchThatClause> suchThatClause1 =
                make_shared<SuchThatClause>(RelationType::FOLLOWS_T, 8, Underscore(), declarations);
        query->suchThatClauses->push_back(suchThatClause1);
        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = FALSE_RESULT;
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select single tuple with two clauses: stmt s1, s2, s3; select s3 such that follows(_, _) and follows(s2, s3)") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("s1");
        Synonym syn2 = Synonym("s2");
        Synonym syn3 = Synonym("s3");
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn1));
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn2));
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn3));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(syn3);

        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);
        shared_ptr<SuchThatClause> suchThatClause1 =
                make_shared<SuchThatClause>(RelationType::FOLLOWS, Underscore(), Underscore(), declarations);
        shared_ptr<SuchThatClause> suchThatClause2 =
                make_shared<SuchThatClause>(RelationType::FOLLOWS, syn2, syn3, declarations);
        query->suchThatClauses->push_back(suchThatClause1);
        query->suchThatClauses->push_back(suchThatClause2);
        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"2", "3", "5", "7", "8"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select two tuple with two clauses: stmt s1, s2, s3; select <s1, s3> such that follows(1, 2) and follows*(2, s3)") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("s1");
        Synonym syn2 = Synonym("s2");
        Synonym syn3 = Synonym("s3");
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn1));
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn2));
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn3));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();

        *returnResults = {syn1, syn3};
        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);
        shared_ptr<SuchThatClause> suchThatClause1 =
                make_shared<SuchThatClause>(RelationType::FOLLOWS, 1, 2, declarations);
        shared_ptr<SuchThatClause> suchThatClause2 =
                make_shared<SuchThatClause>(RelationType::FOLLOWS_T, 2, syn3, declarations);
        query->suchThatClauses->push_back(suchThatClause1);
        query->suchThatClauses->push_back(suchThatClause2);
        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"1 3", "2 3", "3 3", "4 3", "5 3", "6 3", "7 3", "8 3",
                                   "1 8", "2 8", "3 8", "4 8", "5 8", "6 8", "7 8", "8 8"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select two tuple with two clauses with empty result: stmt s1, s2, s3; select <s1, s3> such that follows(1, 3) and follows*(2, s3)") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("s1");
        Synonym syn2 = Synonym("s2");
        Synonym syn3 = Synonym("s3");
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn1));
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn2));
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn3));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();

        *returnResults = {syn1, syn3};
        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);
        shared_ptr<SuchThatClause> suchThatClause1 =
                make_shared<SuchThatClause>(RelationType::FOLLOWS, 1, 3, declarations);
        shared_ptr<SuchThatClause> suchThatClause2 =
                make_shared<SuchThatClause>(RelationType::FOLLOWS_T, 2, syn3, declarations);
        query->suchThatClauses->push_back(suchThatClause1);
        query->suchThatClauses->push_back(suchThatClause2);
        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = EMPTY_RESULT;
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select four tuple with three clauses: stmt s1, s2; variable v; constant c; select <s1, c, s2, v> such that parent(3, s2) and modifies(s1, 'x') and uses(s2, 'y')" ) {
        //s2 7
        //s1 1 3 4 6
        // x y
        //0 1 2
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("s1");
        Synonym syn2 = Synonym("s2");
        Synonym syn3 = Synonym("v");
        Synonym syn4 = Synonym("c");
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn1));
        declarations->push_back(Declaration(QB::DesignEntity::STMT, syn2));
        declarations->push_back(Declaration(QB::DesignEntity::VARIABLE, syn3));
        declarations->push_back(Declaration(QB::DesignEntity::CONSTANT, syn4));

        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();

        *returnResults = {syn1, syn4, syn2, syn3};
        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);
        shared_ptr<SuchThatClause> suchThatClause1 =
                make_shared<SuchThatClause>(RelationType::PARENT, 3, syn2, declarations);
        shared_ptr<SuchThatClause> suchThatClause2 =
                make_shared<SuchThatClause>(RelationType::MODIFIES_S, syn1, Ident("x"), declarations);
        shared_ptr<SuchThatClause> suchThatClause3 =
                make_shared<SuchThatClause>(RelationType::USES_S, syn2, Ident("y"), declarations);
        query->suchThatClauses->push_back(suchThatClause1);
        query->suchThatClauses->push_back(suchThatClause2);
        query->suchThatClauses->push_back(suchThatClause3);
        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = { "1 0 7 x", "3 0 7 x", "4 0 7 x", "6 0 7 x", "1 0 7 y", "3 0 7 y", "6 0 7 y", "4 0 7 y",
                                    "1 1 7 x", "3 1 7 x", "4 1 7 x", "6 1 7 x", "1 1 7 y", "3 1 7 y", "6 1 7 y", "4 1 7 y",
                                    "1 2 7 x", "3 2 7 x", "4 2 7 x", "6 2 7 x", "1 2 7 y", "3 2 7 y", "6 2 7 y", "4 2 7 y"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
}