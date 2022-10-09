//
// Created by Nafour on 2/10/22.
//
#include "catch.hpp"
#include "../../../unit_testing/src/design_extractor/Dummies.h"
#include "design_extractor/DesignExtractor.h"
#include "pkb/PKBStorage.h"
#include "pkb/DataRetriever.h"
#include "query_evaluator/QueryEvaluator.h"
#include "../../../unit_testing/src/query_evaluator/QETestUtils.h"
#include "query_builder/clauses/such_that_clauses/UsesSClause.h"
#include "query_builder/clauses/such_that_clauses/ModifiesSClause.h"
#include "../../../unit_testing/src/query_evaluator/Dummies/TestQueryBuilder.h"

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

        Synonym syn1 = Synonym("v");

        auto builder = make_shared<TestQE::TestQueryBuilder>();
        auto testQuery = builder
                ->addDeclaration(QB::DesignEntity::VARIABLE, syn1)
                ->addToSelect(syn1)
                ->build();

        auto actual= queryEvaluator->evaluate(testQuery);
        vector<string> expected = {"x", "y"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("variable v; select Boolean from procedure 2: true result") {
        auto pNode = TestDE::Dummies::getTestProgramNode(2 - PROGRAM_NODE_IDX_OFFSET);
        shared_ptr<DE::DesignExtractor> designExtractor = make_shared<DE::DesignExtractor>(dataModifier, pNode);
        designExtractor->run();

        Synonym syn1 = Synonym("v");

        auto builder = make_shared<TestQE::TestQueryBuilder>();
        auto testQuery = builder
                ->addDeclaration(QB::DesignEntity::VARIABLE, syn1)
                ->setReturnBoolean()
                ->build();

        auto actual= queryEvaluator->evaluate(testQuery);
        vector<string> expected = {"TRUE"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("variable v; select Boolean such that Modifies(2, 'x') from procedure 2: false result") {
        auto pNode = TestDE::Dummies::getTestProgramNode(2 - PROGRAM_NODE_IDX_OFFSET);
        shared_ptr<DE::DesignExtractor> designExtractor = make_shared<DE::DesignExtractor>(dataModifier, pNode);
        designExtractor->run();

        Synonym syn1 = Synonym("v");

        auto builder = make_shared<TestQE::TestQueryBuilder>();
        auto testQuery = builder
                ->addDeclaration(QB::DesignEntity::VARIABLE, syn1)
                ->setReturnBoolean()
                ->addModifiesS(2, Ident("x"))
                ->build();

        auto actual= queryEvaluator->evaluate(testQuery);
        vector<string> expected = {"FALSE"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("variable v; select <v, v> from procedure 2") {
        auto pNode = TestDE::Dummies::getTestProgramNode(2 - PROGRAM_NODE_IDX_OFFSET);
        shared_ptr<DE::DesignExtractor> designExtractor = make_shared<DE::DesignExtractor>(dataModifier, pNode);
        designExtractor->run();

        Synonym syn1 = Synonym("v");

        auto builder = make_shared<TestQE::TestQueryBuilder>();
        auto query = builder->addDeclaration(QB::DesignEntity::VARIABLE, syn1)
                ->addToSelect(syn1)
                ->addToSelect(syn1)
                ->build();

        auto actual= queryEvaluator->evaluate(query);
        vector<string> expected = {"x x", "y y"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("print pr; select pr with pr.varName = 'x'; from procedure 3") {
        auto pNode = TestDE::Dummies::getTestProgramNode(3 - PROGRAM_NODE_IDX_OFFSET);
        shared_ptr<DE::DesignExtractor> designExtractor = make_shared<DE::DesignExtractor>(dataModifier, pNode);
        designExtractor->run();

        Synonym syn1 = Synonym("pr");

        auto query = make_shared<TestQE::TestQueryBuilder>()->addDeclaration(QB::DesignEntity::PRINT, syn1)
                ->addToSelect(syn1)
                ->addWith(AttrRef(syn1, QB::AttrName::VAR_NAME), Ident("x"))
                ->build();

        auto actual= queryEvaluator->evaluate(query);
        vector<string> expected = {"1"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("print pr; select pr.varName such that Uses(pr, 'x'); from procedure 3") {
        auto pNode = TestDE::Dummies::getTestProgramNode(3 - PROGRAM_NODE_IDX_OFFSET);
        shared_ptr<DE::DesignExtractor> designExtractor = make_shared<DE::DesignExtractor>(dataModifier, pNode);
        designExtractor->run();

        Synonym syn1 = Synonym("pr");

        auto query = make_shared<TestQE::TestQueryBuilder>()->addDeclaration(QB::DesignEntity::PRINT, syn1)
                ->addToSelect(AttrRef(syn1, QB::AttrName::VAR_NAME))
                ->addUsesS(syn1, Ident("x"))
                ->build();

        auto actual= queryEvaluator->evaluate(query);
        vector<string> expected = {"x"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("print pr; read rd; select <rd, pr.varName> such that Uses(pr, 'x') and Modifies(rd, _); from procedure 3") {
        auto pNode = TestDE::Dummies::getTestProgramNode(3 - PROGRAM_NODE_IDX_OFFSET);
        shared_ptr<DE::DesignExtractor> designExtractor = make_shared<DE::DesignExtractor>(dataModifier, pNode);
        designExtractor->run();

        Synonym syn1 = Synonym("pr");
        Synonym syn2 = Synonym("rd");

        auto query = make_shared<TestQE::TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::PRINT, syn1)
                ->addDeclaration(QB::DesignEntity::READ, syn2)
                ->addToSelect(syn2)
                ->addToSelect(AttrRef(syn1, QB::AttrName::VAR_NAME))
                ->addUsesS(syn1, Ident("x"))
                ->addModifiesS(syn2, Underscore())
                ->build();

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

        Synonym syn1 = Synonym("pr");
        Synonym syn2 = Synonym("rd");
        Synonym syn3 = Synonym("v");

        auto query = make_shared<TestQE::TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::PRINT, syn1)
                ->addDeclaration(QB::DesignEntity::READ, syn2)
                ->addDeclaration(QB::DesignEntity::VARIABLE, syn3)
                ->addToSelect(syn2)
                ->addToSelect(AttrRef(syn1, QB::AttrName::VAR_NAME))
                ->addToSelect(syn3)
                ->addUsesS(syn1, Underscore())
                ->addModifiesS(syn2, Ident("foo"))
                ->build();

        //rd: 7
        //pr: 1 5
        //v: x y bar z w foo
        auto actual= queryEvaluator->evaluate(query);
        vector<string> expected = {"7 x x", "7 x y", "7 x bar", "7 x z", "7 x w", "7 x foo",
                                   "7 z x", "7 z y", "7 z bar", "7 z z", "7 z w", "7 z foo"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("print pr; select pr with pr.varname == 'x'; from procedure 3") {
        auto pNode = TestDE::Dummies::getTestProgramNode(3 - PROGRAM_NODE_IDX_OFFSET);
        shared_ptr<DE::DesignExtractor> designExtractor = make_shared<DE::DesignExtractor>(dataModifier, pNode);
        designExtractor->run();

        Synonym syn1 = Synonym("pr");

        auto query = make_shared<TestQE::TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::PRINT, syn1)
                ->addToSelect(syn1)
                ->addWith(AttrRef(syn1, QB::AttrName::VAR_NAME), Ident("x"))
                ->build();

        auto actual= queryEvaluator->evaluate(query);
        vector<string> expected = {"1"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("print pr; select pr.varname with pr.varname == 'x'; from procedure 3") {
        auto pNode = TestDE::Dummies::getTestProgramNode(3 - PROGRAM_NODE_IDX_OFFSET);
        shared_ptr<DE::DesignExtractor> designExtractor = make_shared<DE::DesignExtractor>(dataModifier, pNode);
        designExtractor->run();

        Synonym syn1 = Synonym("pr");

        auto query = make_shared<TestQE::TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::PRINT, syn1)
                ->addToSelect(AttrRef(syn1, QB::AttrName::VAR_NAME))
                ->addWith(AttrRef(syn1, QB::AttrName::VAR_NAME), Ident("x"))
                ->build();

        auto actual= queryEvaluator->evaluate(query);
        vector<string> expected = {"x"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    //test such that + with
    SECTION("variable v; print pr; if ifs; Select pr with v.varname = pr.varname and parent*(ifs, pr); from procedure 4") {
        auto pNode = TestDE::Dummies::getTestProgramNode(4 - PROGRAM_NODE_IDX_OFFSET);
        shared_ptr<DE::DesignExtractor> designExtractor = make_shared<DE::DesignExtractor>(dataModifier, pNode);
        designExtractor->run();

        Synonym syn1 = Synonym("v");
        Synonym syn2 = Synonym("pr");
        Synonym syn3 = Synonym("ifs");

        auto query = make_shared<TestQE::TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::VARIABLE, syn1)
                ->addDeclaration(QB::DesignEntity::PRINT, syn2)
                ->addDeclaration(QB::DesignEntity::IF, syn3)
                ->addToSelect(syn2)
                ->addWith(AttrRef(syn1, QB::AttrName::VAR_NAME), AttrRef(syn2, QB::AttrName::VAR_NAME))
                ->addParentT(syn3, syn2)
                ->build();

        auto actual= queryEvaluator->evaluate(query);
        vector<string> expected = {"4", "7"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("variable v; print pr; if ifs; Select ifs with v.varname = pr.varname and parent(ifs, pr); from procedure 4") {
        auto pNode = TestDE::Dummies::getTestProgramNode(4 - PROGRAM_NODE_IDX_OFFSET);
        shared_ptr<DE::DesignExtractor> designExtractor = make_shared<DE::DesignExtractor>(dataModifier, pNode);
        designExtractor->run();

        Synonym syn1 = Synonym("v");
        Synonym syn2 = Synonym("pr");
        Synonym syn3 = Synonym("ifs");

        auto query = make_shared<TestQE::TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::VARIABLE, syn1)
                ->addDeclaration(QB::DesignEntity::PRINT, syn2)
                ->addDeclaration(QB::DesignEntity::IF, syn3)
                ->addToSelect(syn3)
                ->addWith(AttrRef(syn1, QB::AttrName::VAR_NAME), AttrRef(syn2, QB::AttrName::VAR_NAME))
                ->addParent(syn3, syn2)
                ->build();

        auto actual= queryEvaluator->evaluate(query);
        vector<string> expected = {"3"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    //test with clause with constant value
    SECTION("constant c; stmt s;  Select s with c.value = s.stmt# ; from procedure 4") {
        auto pNode = TestDE::Dummies::getTestProgramNode(4 - PROGRAM_NODE_IDX_OFFSET);
        shared_ptr<DE::DesignExtractor> designExtractor = make_shared<DE::DesignExtractor>(dataModifier, pNode);
        designExtractor->run();

        Synonym syn1 = Synonym("c");
        Synonym syn2 = Synonym("s");

        auto query = make_shared<TestQE::TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::CONSTANT, syn1)
                ->addDeclaration(QB::DesignEntity::STMT, syn2)
                ->addToSelect(syn2)
                ->addWith(AttrRef(syn1, QB::AttrName::VALUE), AttrRef(syn2, QB::AttrName::STMT_NUMBER))
                ->build();

        auto actual= queryEvaluator->evaluate(query);
        vector<string> expected = {"3", "1", "5"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
        //test with clause + such that clause with constant value
    SECTION("constant c; stmt s;  Select s with c.value = s.stmt# and such that parent(s, 4); from procedure 4") {
        auto pNode = TestDE::Dummies::getTestProgramNode(4 - PROGRAM_NODE_IDX_OFFSET);
        shared_ptr<DE::DesignExtractor> designExtractor = make_shared<DE::DesignExtractor>(dataModifier, pNode);
        designExtractor->run();

        Synonym syn1 = Synonym("c");
        Synonym syn2 = Synonym("s");

        auto query = make_shared<TestQE::TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::CONSTANT, syn1)
                ->addDeclaration(QB::DesignEntity::STMT, syn2)
                ->addToSelect(syn2)
                ->addParent(syn2, 4)
                ->addWith(AttrRef(syn1, QB::AttrName::VALUE), AttrRef(syn2, QB::AttrName::STMT_NUMBER))
                ->build();

        auto actual= queryEvaluator->evaluate(query);
        vector<string> expected = {"3"};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
}