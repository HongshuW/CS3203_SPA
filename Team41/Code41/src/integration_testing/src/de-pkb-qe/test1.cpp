//
// Created by Nafour on 2/10/22.
//
#include "catch.hpp"
#include "../../../unit_testing/src/design_extractor/Dummies.h"
#include "design_extractor/DesignExtractor.h"
#include "pkb/PKBStorage.h"
#include "pkb/DataRetriever.h"
#include "query_evaluator/QueryEvaluator.h"
#include "query_builder/commons/Query.h"
#include "../../../unit_testing/src/query_evaluator/QETestUtils.h"
#include "query_builder/clauses/such_that_clauses/UsesSClause.h"
#include "query_builder/clauses/such_that_clauses/ModifiesSClause.h"

TEST_CASE("Test de-pkb-qe integration") {
    const int PROGRAM_NODE_IDX_OFFSET = 1;
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    shared_ptr<DataRetriever> dataRetriever = make_shared<DataRetriever>(pkb);
    shared_ptr<QE::QueryEvaluator> queryEvaluator = make_shared<QE::QueryEvaluator>(dataRetriever);

    SECTION("variable v; select v from procedure 2") {
        auto pNode = TestDE::Dummies::getTestProgramNode(2 - PROGRAM_NODE_IDX_OFFSET);
        shared_ptr<DE::DesignExtractor> designExtractor = make_shared<DE::DesignExtractor>(dataModifier, pNode);
        designExtractor->run();

        shared_ptr<Query> query = make_shared<Query>();
        Synonym syn1 = Synonym("v");

        Declarations declarations = make_shared<vector<Declaration>>();
        declarations->push_back(Declaration(DesignEntity::VARIABLE, syn1));
        query->declarations = declarations;

        Tuple tuple = make_shared<vector<Elem>>();
        tuple->push_back(syn1);
        shared_ptr<SelectClause> selectClause1 = make_shared<SelectClause>(ReturnType::TUPLE, tuple);
        query->selectClause = selectClause1;

        auto actual= queryEvaluator->evaluate(query);
        vector<string> expected = {"x", "y"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("variable v; select <v, v> from procedure 2") {
        auto pNode = TestDE::Dummies::getTestProgramNode(2 - PROGRAM_NODE_IDX_OFFSET);
        shared_ptr<DE::DesignExtractor> designExtractor = make_shared<DE::DesignExtractor>(dataModifier, pNode);
        designExtractor->run();

        shared_ptr<Query> query = make_shared<Query>();
        Synonym syn1 = Synonym("v");

        Declarations declarations = make_shared<vector<Declaration>>();
        declarations->push_back(Declaration(DesignEntity::VARIABLE, syn1));
        query->declarations = declarations;

        Tuple tuple = make_shared<vector<Elem>>();
        tuple->push_back(syn1);
        tuple->push_back(syn1);
        shared_ptr<SelectClause> selectClause1 = make_shared<SelectClause>(ReturnType::TUPLE, tuple);
        query->selectClause = selectClause1;

        auto actual= queryEvaluator->evaluate(query);
        vector<string> expected = {"x x", "y y"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("print pr; select pr with pr.varName = 'x'; from procedure 3") {
        auto pNode = TestDE::Dummies::getTestProgramNode(3 - PROGRAM_NODE_IDX_OFFSET);
        shared_ptr<DE::DesignExtractor> designExtractor = make_shared<DE::DesignExtractor>(dataModifier, pNode);
        designExtractor->run();

        shared_ptr<Query> query = make_shared<Query>();
        Synonym syn1 = Synonym("pr");

        Declarations declarations = make_shared<vector<Declaration>>();
        declarations->push_back(Declaration(DesignEntity::PRINT, syn1));
        query->declarations = declarations;

        Tuple tuple = make_shared<vector<Elem>>();
        tuple->push_back(syn1);
        shared_ptr<SelectClause> selectClause1 = make_shared<SelectClause>(ReturnType::TUPLE, tuple);
        query->selectClause = selectClause1;

        shared_ptr<WithClause> withClause = make_shared<WithClause>(AttrRef(syn1, QB::AttrName::VAR_NAME), Ident("x"));
        query->withClauses->push_back(withClause);
        auto actual= queryEvaluator->evaluate(query);
        vector<string> expected = {"1"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("print pr; select pr.varName such that Uses(pr, 'x'); from procedure 3") {
        auto pNode = TestDE::Dummies::getTestProgramNode(3 - PROGRAM_NODE_IDX_OFFSET);
        shared_ptr<DE::DesignExtractor> designExtractor = make_shared<DE::DesignExtractor>(dataModifier, pNode);
        designExtractor->run();

        shared_ptr<Query> query = make_shared<Query>();
        Synonym syn1 = Synonym("pr");

        Declarations declarations = make_shared<vector<Declaration>>();
        declarations->push_back(Declaration(DesignEntity::PRINT, syn1));
        query->declarations = declarations;

        Tuple tuple = make_shared<vector<Elem>>();
        tuple->push_back(AttrRef(syn1, QB::AttrName::VAR_NAME));

        shared_ptr<SelectClause> selectClause1 = make_shared<SelectClause>(ReturnType::TUPLE, tuple);
        query->selectClause = selectClause1;

        shared_ptr<UsesSClause> usesSClause = make_shared<UsesSClause>(syn1, Ident("x"));
        query->suchThatClauses->push_back(usesSClause);

        auto actual= queryEvaluator->evaluate(query);
        vector<string> expected = {"x"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("print pr; read rd; select <rd, pr.varName> such that Uses(pr, 'x') and Modifies(rd, _); from procedure 3") {
        auto pNode = TestDE::Dummies::getTestProgramNode(3 - PROGRAM_NODE_IDX_OFFSET);
        shared_ptr<DE::DesignExtractor> designExtractor = make_shared<DE::DesignExtractor>(dataModifier, pNode);
        designExtractor->run();

        shared_ptr<Query> query = make_shared<Query>();
        Synonym syn1 = Synonym("pr");
        Synonym syn2 = Synonym("rd");

        Declarations declarations = make_shared<vector<Declaration>>();
        declarations->push_back(Declaration(DesignEntity::PRINT, syn1));
        declarations->push_back(Declaration(QB::DesignEntity::READ, syn2));
        query->declarations = declarations;

        Tuple tuple = make_shared<vector<Elem>>();
        tuple->push_back(syn2);
        tuple->push_back(AttrRef(syn1, QB::AttrName::VAR_NAME));

        shared_ptr<SelectClause> selectClause1 = make_shared<SelectClause>(ReturnType::TUPLE, tuple);
        query->selectClause = selectClause1;

        shared_ptr<UsesSClause> usesSClause = make_shared<UsesSClause>(syn1, Ident("x"));
        shared_ptr<ModifiesSClause> modifiesSClause = make_shared<ModifiesSClause>(syn2, Underscore());
        query->suchThatClauses->push_back(usesSClause);
        query->suchThatClauses->push_back(modifiesSClause);

        //rd: 2 6 7
        //pr: 1
        auto actual= queryEvaluator->evaluate(query);
        vector<string> expected = {"2 x", "6 x", "7 x"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("print pr; read rd; variable v; select <rd, pr.varName, v> such that Uses(pr, _) and Modifies(rd, 'foo'); from procedure 3") {
        auto pNode = TestDE::Dummies::getTestProgramNode(3 - PROGRAM_NODE_IDX_OFFSET);
        shared_ptr<DE::DesignExtractor> designExtractor = make_shared<DE::DesignExtractor>(dataModifier, pNode);
        designExtractor->run();

        shared_ptr<Query> query = make_shared<Query>();
        Synonym syn1 = Synonym("pr");
        Synonym syn2 = Synonym("rd");
        Synonym syn3 = Synonym("v");

        Declarations declarations = make_shared<vector<Declaration>>();
        declarations->push_back(Declaration(DesignEntity::PRINT, syn1));
        declarations->push_back(Declaration(QB::DesignEntity::READ, syn2));
        declarations->push_back(Declaration(QB::DesignEntity::VARIABLE, syn3));
        query->declarations = declarations;

        Tuple tuple = make_shared<vector<Elem>>();
        tuple->push_back(syn2);
        tuple->push_back(AttrRef(syn1, QB::AttrName::VAR_NAME));
        tuple->push_back(syn3);

        shared_ptr<SelectClause> selectClause1 = make_shared<SelectClause>(ReturnType::TUPLE, tuple);
        query->selectClause = selectClause1;

        shared_ptr<UsesSClause> usesSClause = make_shared<UsesSClause>(syn1, Underscore());
        shared_ptr<ModifiesSClause> modifiesSClause = make_shared<ModifiesSClause>(syn2, Ident("foo"));
        query->suchThatClauses->push_back(usesSClause);
        query->suchThatClauses->push_back(modifiesSClause);

        //rd: 7
        //pr: 1 5
        //v: x y bar z w foo
        auto actual= queryEvaluator->evaluate(query);
        vector<string> expected = {"7 x x", "7 x y", "7 x bar", "7 x z", "7 x w", "7 x foo",
                                   "7 z x", "7 z y", "7 z bar", "7 z z", "7 z w", "7 z foo"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
}