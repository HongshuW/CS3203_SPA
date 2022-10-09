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
#include "Dummies/TestQueryBuilder.h"

using namespace std;
using namespace TestQE;
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
        Synonym syn1 = Synonym("v");
        auto query = make_shared<TestQE::TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::VARIABLE, syn1)
                ->addToSelect(syn1)
                ->addModifiesS(1, syn1)
                ->build();
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"dummyVarA"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select single tuple: variable v; select v such that modifies('p1', v)") {
        Synonym syn1 = Synonym("v");
        auto query = make_shared<TestQE::TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::VARIABLE, syn1)
                ->addToSelect(syn1)
                ->addModifiesP(Ident("p1"), syn1)
                ->build();
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"dummyVarA", "dummyVarB"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select single tuple: stmt s; select s such that modifies(s, _)") {
        Synonym syn1 = Synonym("s");
        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addToSelect(syn1)
                ->addModifiesS(syn1, Underscore())
                ->build();

        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"1", "2"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select single tuple: stmt s; select s such that follows*(_, 3)") {
        Synonym syn1 = Synonym("s");
        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addToSelect(syn1)
                ->addFollowsT(Underscore(), 3)
                ->build();

        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"1", "2", "3"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return true: stmt s; select bool such that follows(_, 3)") {
        Synonym syn1 = Synonym("s");
        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->setReturnBoolean()
                ->addFollows(Underscore(), 3)
                ->build();
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = TRUE_RESULT;
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return false: assign a; select bool such that follows*(_, 1)") {
        Synonym syn1 = Synonym("a");
        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::ASSIGN, syn1)
                ->setReturnBoolean()
                ->addFollowsT(Underscore(), 1)
                ->build();
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = FALSE_RESULT;
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return false: select bool with 1 == 2") {

        auto query = make_shared<TestQueryBuilder>()
                ->setReturnBoolean()
                ->addWith(1,2)
                ->build();
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = FALSE_RESULT;
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return true: select bool with 'x' == 'x'") {

        auto query = make_shared<TestQueryBuilder>()
                ->setReturnBoolean()
                ->addWith(Ident("x"),Ident("x"))
                ->build();
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = TRUE_RESULT;
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return true: variable v; select bool with v.varname == v.varname") {
        Synonym syn1 = Synonym("v");
        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::VARIABLE, syn1)
                ->setReturnBoolean()
                ->addWith(AttrRef(syn1, QB::AttrName::VAR_NAME), AttrRef(syn1, QB::AttrName::VAR_NAME))
                ->build();
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = TRUE_RESULT;
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return true: stmt s1, s2; select <s1, s2> with s1.stmt# == s2.stmt#") {
        Synonym syn1 = Synonym("s1");
        Synonym syn2 = Synonym("s2");
        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addDeclaration(QB::DesignEntity::STMT, syn2)
                ->addToSelect(syn1)
                ->addToSelect(syn2)
                ->addWith(AttrRef(syn1, QB::AttrName::STMT_NUMBER), AttrRef(syn2, QB::AttrName::STMT_NUMBER))
                ->build();
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"1 1", "2 2", "3 3"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return true: select bool such that follows(1, 2)") {

        auto query = make_shared<TestQueryBuilder>()
                ->setReturnBoolean()
                ->addFollows(1,2)
                ->build();
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = TRUE_RESULT;
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return false: procedure p; select bool such that uses(p, 'y')") {
        Synonym syn1 = Synonym("p");
        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::PROCEDURE, syn1)
                ->setReturnBoolean()
                ->addUsesP(syn1, Ident("y"))
                ->build();
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = FALSE_RESULT;
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select procedure: procedure p; variable v; select p such that uses(p, v)") {
        Synonym syn1 = Synonym("p");
        Synonym syn2 = Synonym("v");
        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::PROCEDURE, syn1)
                ->addDeclaration(QB::DesignEntity::VARIABLE, syn2)
                ->addToSelect(syn1)
                ->addUsesP(syn1, syn2)
                ->build();
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"p1"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return variable: procedure p; variable v; select v such that modifies(p, v)") {
        Synonym syn1 = Synonym("p");
        Synonym syn2 = Synonym("v");
        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::PROCEDURE, syn1)
                ->addDeclaration(QB::DesignEntity::VARIABLE, syn2)
                ->addToSelect(syn2)
                ->addModifiesP(syn1, syn2)
                ->build();
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"dummyVarA", "dummyVarB"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select single tuple with two clauses: stmt s1, s2, s3; select s2 such that follows(s1, s2) and follows(s2, s3)") {
        Synonym syn1 = Synonym("s1");
        Synonym syn2 = Synonym("s2");
        Synonym syn3 = Synonym("s3");

        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addDeclaration(QB::DesignEntity::STMT, syn2)
                ->addDeclaration(QB::DesignEntity::STMT, syn3)
                ->addToSelect(syn2)
                ->addFollows(syn1, syn2)
                ->addFollows(syn2, syn3)
                ->build();
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"2"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select two tuple with two clauses: stmt s1, s2, s3; select <s1,s2> such that follows(s1, s2) and follows(s2, s3)") {
        Synonym syn1 = Synonym("s1");
        Synonym syn2 = Synonym("s2");
        Synonym syn3 = Synonym("s3");

        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addDeclaration(QB::DesignEntity::STMT, syn2)
                ->addDeclaration(QB::DesignEntity::STMT, syn3)
                ->addToSelect(syn1)
                ->addToSelect(syn2)
                ->addFollows(syn1, syn2)
                ->addFollows(syn2, syn3)
                ->build();
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"1 2"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select three tuple with two clauses: stmt s1, s2; variable v; select <s1, s2, v> such that follows(s1, s2) and uses(s2, _)") {
        Synonym syn1 = Synonym("s1");
        Synonym syn2 = Synonym("s2");
        Synonym syn3 = Synonym("v");

        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addDeclaration(QB::DesignEntity::STMT, syn2)
                ->addDeclaration(QB::DesignEntity::VARIABLE, syn3)
                ->addToSelect(syn1)
                ->addToSelect(syn2)
                ->addToSelect(syn3)
                ->addFollows(syn1, syn2)
                ->addUsesS(syn2, Underscore())
                ->build();

        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"1 2 dummyVarA", "2 3 dummyVarA", "1 2 dummyVarB", "2 3 dummyVarB"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select four tuple with three clauses: stmt s1, s2; variable v; constant c; select <s1, c, s2, v> such that follows(s1, s2) and uses(s2, _) and modifies(s1, 'dummyVarB')") {
        Synonym syn1 = Synonym("s1");
        Synonym syn2 = Synonym("s2");
        Synonym syn3 = Synonym("v");
        Synonym syn4 = Synonym("c");

        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addDeclaration(QB::DesignEntity::STMT, syn2)
                ->addDeclaration(QB::DesignEntity::VARIABLE, syn3)
                ->addDeclaration(QB::DesignEntity::CONSTANT, syn4)
                ->addToSelect(syn1)
                ->addToSelect(syn4)
                ->addToSelect(syn2)
                ->addToSelect(syn3)
                ->addFollows(syn1, syn2)
                ->addUsesS(syn2, Underscore())
                ->addModifiesS(syn1, Ident("dummyVarB"))
                ->build();
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = { "2 1 3 dummyVarB", "2 1 3 dummyVarA"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select single with one pattern clause: assign a; select a pattern a('dummyVarA', _)") {
        Synonym syn1 = Synonym("a");
        ExpressionSpec expressionSpec = ExpressionSpec(QB::ExpressionSpecType::ANY_MATCH);

        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::ASSIGN, syn1)
                ->addToSelect(syn1)
                ->addAssignPattern(syn1, Ident("dummyVarA"), expressionSpec)
                ->build();
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"1"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select single tuple with one pattern clause and one such that: assign a; variable v; select a pattern a(v, _) such that follows(1, a)") {
        Synonym syn1 = Synonym("a");
        Synonym syn2 = Synonym("v");
        ExpressionSpec expressionSpec = ExpressionSpec(QB::ExpressionSpecType::ANY_MATCH);

        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::ASSIGN, syn1)
                ->addDeclaration(QB::DesignEntity::VARIABLE, syn2)
                ->addToSelect(syn1)
                ->addAssignPattern(syn1, syn2, expressionSpec)
                ->addFollows(1, syn1)
                ->build();

        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"2"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select two tuple with one pattern clause and one such that: assign a; variable v; select <v, a> pattern a(v, _) such that follows(1, a)") {
        Synonym syn1 = Synonym("a");
        Synonym syn2 = Synonym("v");
        ExpressionSpec expressionSpec = ExpressionSpec(QB::ExpressionSpecType::ANY_MATCH);


        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::ASSIGN, syn1)
                ->addDeclaration(QB::DesignEntity::VARIABLE, syn2)
                ->addToSelect(syn2)
                ->addToSelect(syn1)
                ->addAssignPattern(syn1, syn2, expressionSpec)
                ->addFollows(1, syn1)
                ->build();

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
    SECTION("test select single tuple: variable v; select v.varname") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("v");
        declarations->push_back(Declaration(QB::DesignEntity::VARIABLE, syn1));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(AttrRef(syn1, QB::AttrName::VAR_NAME));

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
    SECTION("test select single tuple with non-empty result: call c; select c") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("c");
        declarations->push_back(Declaration(QB::DesignEntity::CALL, syn1));
        query->declarations = declarations;
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(syn1);

        query->selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);

        auto actual = queryEvaluator2.evaluate(query);
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, {"7"}));
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
        vector<string> expected = {"x 4", "x 6", "y 4", "y 6",};
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
        vector<string> expected = {"4 x", "6 x", "4 y", "6 y"};
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
        vector<string> expected = {"x x","y y"};
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
        Synonym syn1 = Synonym("s");
        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addToSelect(syn1)
                ->addParent(Underscore(), syn1)
                ->build();
        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"4", "5","6", "7"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));

    }
    SECTION("test select single tuple: read r; select r such that uses(3, 'y')") {
        Synonym syn1 = Synonym("r");

        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::READ, syn1)
                ->addToSelect(syn1)
                ->addUsesS(3, Ident("y"))
                ->build();
        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"1", "2"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return false: assign a; select bool such that parent(8, _)") {
        Synonym syn1 = Synonym("a");

        auto query = make_shared<TestQueryBuilder>()
                ->setReturnBoolean()
                ->addDeclaration(QB::DesignEntity::ASSIGN, syn1)
                ->addParent(8, Underscore())
                ->build();
        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = FALSE_RESULT;
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return false: if ifs; select bool such that pattern ifs('y', _, _)") {
        Synonym syn1 = Synonym("ifs");

        auto query = make_shared<TestQueryBuilder>()
                ->setReturnBoolean()
                ->addDeclaration(QB::DesignEntity::IF, syn1)
                ->addIfPattern(syn1, Ident("y"))
                ->build();
        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = FALSE_RESULT;
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return false: while w; select w such that pattern w('y', _)") {
        Synonym syn1 = Synonym("w");

        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::WHILE, syn1)
                ->addToSelect(syn1)
                ->addWhilePattern(syn1, Ident("y"))
                ->build();
        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"3"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return false: while w; variable v; select v such that pattern w(v, _)") {
        Synonym syn1 = Synonym("w");
        Synonym syn2 = Synonym("v");

        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::WHILE, syn1)
                ->addToSelect(syn2)
                ->addWhilePattern(syn1, syn2)
                ->build();
        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"y"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select stmt : stmt s1, s2; select s2.stmt# such that parentT(s1, s2)") {
        Synonym syn1 = Synonym("s1");
        Synonym syn2 = Synonym("s2");

        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addDeclaration(QB::DesignEntity::STMT, syn2)
                ->addToSelect(AttrRef(syn2, AttrName::STMT_NUMBER))
                ->addParentT(syn1, syn2)
                ->build();
        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"4", "5", "6", "7"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select stmt : stmt s1, s2; select s1 such that parentT(s1, s2) and with s2.stmt# == 6") {
        Synonym syn1 = Synonym("s1");
        Synonym syn2 = Synonym("s2");

        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addDeclaration(QB::DesignEntity::STMT, syn2)
                ->addToSelect(syn1)
                ->addParentT(syn1, syn2)
                ->addWith(AttrRef(syn2, QB::AttrName::STMT_NUMBER), 6)
                ->build();
        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"3", "5"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select stmt : stmt s1, s2; select s1 such that parentT(s1, s2) and with 6 == s2.stmt#") {
        Synonym syn1 = Synonym("s1");
        Synonym syn2 = Synonym("s2");

        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addDeclaration(QB::DesignEntity::STMT, syn2)
                ->addToSelect(syn1)
                ->addParentT(syn1, syn2)
                ->addWith(6, AttrRef(syn2, QB::AttrName::STMT_NUMBER))
                ->build();
        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"3", "5"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select stmt : stmt s1; variable v; select s1 such that modifies(s1, v) and with v.varname == 'x'") {
        Synonym syn1 = Synonym("s1");
        Synonym syn2 = Synonym("v");

        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addDeclaration(QB::DesignEntity::VARIABLE, syn2)
                ->addToSelect(syn1)
                ->addModifiesS(syn1, syn2)
                ->addWith(AttrRef(syn2, QB::AttrName::VAR_NAME), Ident("x"))
                ->build();
        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"3", "1", "4", "6"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select stmt : stmt s1; variable v; select s1 such that modifies(s1, v) and with 'x' == v.varname") {
        Synonym syn1 = Synonym("s1");
        Synonym syn2 = Synonym("v");

        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addDeclaration(QB::DesignEntity::VARIABLE, syn2)
                ->addToSelect(syn1)
                ->addModifiesS(syn1, syn2)
                ->addWith(Ident("x"), AttrRef(syn2, QB::AttrName::VAR_NAME))
                ->build();
        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"3", "1", "4", "6"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select single tuple with two clauses: stmt s1, s2, s3; select s3 such that follows(_, _) and follows(s2, s3)") {
        Synonym syn1 = Synonym("s1");
        Synonym syn2 = Synonym("s2");
        Synonym syn3 = Synonym("s3");

        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addDeclaration(QB::DesignEntity::STMT, syn2)
                ->addDeclaration(QB::DesignEntity::STMT, syn3)
                ->addToSelect(syn3)
                ->addFollows(Underscore(), Underscore())
                ->addFollows(syn2, syn3)
                ->build();
        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"2", "3", "5", "7", "8"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select two tuple with two clauses: stmt s1, s2, s3; select <s1, s3> such that follows(1, 2) and follows*(2, s3)") {
        Synonym syn1 = Synonym("s1");
        Synonym syn2 = Synonym("s2");
        Synonym syn3 = Synonym("s3");

        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addDeclaration(QB::DesignEntity::STMT, syn2)
                ->addDeclaration(QB::DesignEntity::STMT, syn3)
                ->addToSelect(syn1)
                ->addToSelect(syn3)
                ->addFollows(1,2)
                ->addFollowsT(2, syn3)
                ->build();
        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"1 3", "2 3", "3 3", "4 3", "5 3", "6 3", "7 3", "8 3",
                                   "1 8", "2 8", "3 8", "4 8", "5 8", "6 8", "7 8", "8 8"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select two tuple with two clauses with empty result: stmt s1, s2, s3; select <s1, s3> such that follows(1, 3) and follows*(2, s3)") {
        Synonym syn1 = Synonym("s1");
        Synonym syn2 = Synonym("s2");
        Synonym syn3 = Synonym("s3");

        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addDeclaration(QB::DesignEntity::STMT, syn2)
                ->addDeclaration(QB::DesignEntity::STMT, syn3)
                ->addToSelect(syn1)
                ->addToSelect(syn3)
                ->addFollows(1,3)
                ->addFollowsT(2, syn3)
                ->build();

        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = EMPTY_RESULT;
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select four tuple with three clauses: stmt s1, s2; variable v; constant c; select <s1, c, s2, v> such that parent(3, cll) and modifies(s1, 'x') and with cll.procname == 'p3'" ) {
        //s2 7
        //s1 1 3 4 6
        // x y
        //0 1 2
        Synonym syn1 = Synonym("s1");
        Synonym syn2 = Synonym("cll");
        Synonym syn3 = Synonym("v");
        Synonym syn4 = Synonym("c");

        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addDeclaration(QB::DesignEntity::CALL, syn2)
                ->addDeclaration(QB::DesignEntity::VARIABLE, syn3)
                ->addDeclaration(QB::DesignEntity::CONSTANT, syn4)
                ->addToSelect(syn1)
                ->addToSelect(syn4)
                ->addToSelect(syn2)
                ->addToSelect(syn3)
                ->addParent(3, syn2)
                ->addModifiesS(syn1, Ident("x"))
                ->addWith(AttrRef(syn2, QB::AttrName::PROC_NAME), Ident("p3"))
                ->build();

        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = { "1 0 7 x", "3 0 7 x", "4 0 7 x", "6 0 7 x", "1 0 7 y", "3 0 7 y", "6 0 7 y", "4 0 7 y",
                                    "1 1 7 x", "3 1 7 x", "4 1 7 x", "6 1 7 x", "1 1 7 y", "3 1 7 y", "6 1 7 y", "4 1 7 y",
                                    "1 2 7 x", "3 2 7 x", "4 2 7 x", "6 2 7 x", "1 2 7 y", "3 2 7 y", "6 2 7 y", "4 2 7 y"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return true: select bool such that next(1, 2)") {

        auto query = make_shared<TestQueryBuilder>()
                ->setReturnBoolean()
                ->addNext(1,2)
                ->build();
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = TRUE_RESULT;
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return true: select bool such that affect(1, 2)") {

        auto query = make_shared<TestQueryBuilder>()
                ->setReturnBoolean()
                ->addAffects(1,2)
                ->build();
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = TRUE_RESULT;
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return true: select bool such that affect(1, 3)") {

        auto query = make_shared<TestQueryBuilder>()
                ->setReturnBoolean()
                ->addAffectsT(1,3)
                ->build();
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = FALSE_RESULT;
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return true: stmt s; select <s, s.stmt#> such that next*(1, s)") {
        Synonym syn1 = Synonym("s");
        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addToSelect(syn1)
                ->addToSelect(AttrRef(syn1, QB::AttrName::STMT_NUMBER))
                ->addNextT(1,syn1)
                ->build();
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"2 2", "3 3"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return true: stmt s; select <s.stmt#, s> such that affect(1, s)") {
        Synonym syn1 = Synonym("s");
        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addToSelect(AttrRef(syn1, QB::AttrName::STMT_NUMBER))
                ->addToSelect(syn1)
                ->addAffects(1,syn1)
                ->build();
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"2 2"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return true: stmt s1, s2; select <s1.stmt#, s2.stmt#> such that affect*(s2, s1)") {
        Synonym syn1 = Synonym("s1");
        Synonym syn2 = Synonym("s2");
        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addDeclaration(QB::DesignEntity::STMT, syn2)
                ->addToSelect(AttrRef(syn1, QB::AttrName::STMT_NUMBER))
                ->addToSelect(AttrRef(syn2, QB::AttrName::STMT_NUMBER))
                ->addAffectsT(syn2,syn1)
                ->build();
        auto actual = queryEvaluator.evaluate(query);
        vector<string> expected = {"2 1"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return true: procedure p; select p.procname such that uses(p, 'x') and calls(p, 'p3')") {
        Synonym syn1 = Synonym("p");
        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::PROCEDURE, syn1)
                ->addToSelect(AttrRef(syn1, QB::AttrName::PROC_NAME))
                ->addUsesP(syn1,Ident("x"))
                ->addCalls(syn1, Ident("p3"))
                ->build();
        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"p2"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return true: procedure p; variable v; select <p.procname, v.varname> such that uses(p, 'y') and calls*(p, 'p3')") {
        Synonym syn1 = Synonym("p");
        Synonym syn2 = Synonym("v");
        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::PROCEDURE, syn1)
                ->addDeclaration(QB::DesignEntity::VARIABLE, syn2)
                ->addToSelect(AttrRef(syn1, QB::AttrName::PROC_NAME))
                ->addToSelect(AttrRef(syn2, QB::AttrName::VAR_NAME))
                ->addUsesP(syn1,Ident("y"))
                ->addCallT(syn1, Ident("p3"))
                ->build();
        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"p2 x", "p2 y"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return true: procedure p; variable v; select <p.procname, v> such that follows(1, 2) ") {
        Synonym syn1 = Synonym("p");
        Synonym syn2 = Synonym("v");
        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::PROCEDURE, syn1)
                ->addDeclaration(QB::DesignEntity::VARIABLE, syn2)
                ->addToSelect(AttrRef(syn1, QB::AttrName::PROC_NAME))
                ->addToSelect(AttrRef(syn2, QB::AttrName::VAR_NAME))
                ->addFollows(1,2)
                ->build();
        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"p2 x", "p2 y"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return true: call cll; variable v; select <cll.procname, v> such that follows(1, 2) ") {
        Synonym syn1 = Synonym("cll");
        Synonym syn2 = Synonym("v");
        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::CALL, syn1)
                ->addDeclaration(QB::DesignEntity::VARIABLE, syn2)
                ->addToSelect(AttrRef(syn1, QB::AttrName::PROC_NAME))
                ->addToSelect(AttrRef(syn2, QB::AttrName::VAR_NAME))
                ->addFollows(1,2)
                ->build();
        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"p3 x", "p3 y"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return true: call cll; variable v; select <cll, v> such that follows(1, 2) ") {
        Synonym syn1 = Synonym("cll");
        Synonym syn2 = Synonym("v");
        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::CALL, syn1)
                ->addDeclaration(QB::DesignEntity::VARIABLE, syn2)
                ->addToSelect(syn1)
                ->addToSelect(AttrRef(syn2, QB::AttrName::VAR_NAME))
                ->addFollows(1,2)
                ->build();
        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"7 x", "7 y"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("test select return true: procedure p; variable v; select <p.procname, v> such that uses(p, _) ") {
        Synonym syn1 = Synonym("p");
        Synonym syn2 = Synonym("v");
        auto query = make_shared<TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::PROCEDURE, syn1)
                ->addDeclaration(QB::DesignEntity::VARIABLE, syn2)
                ->addToSelect(AttrRef(syn1, QB::AttrName::PROC_NAME))
                ->addToSelect(AttrRef(syn2, QB::AttrName::VAR_NAME))
                ->addUsesP(syn1,Underscore())
                ->build();
        auto actual = queryEvaluator2.evaluate(query);
        vector<string> expected = {"p2 x", "p2 y"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }

}