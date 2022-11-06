//
// Created by Nafour on 2/10/22.
//
#include "../../../unit_testing/src/design_extractor/Dummies.h"
#include "../../../unit_testing/src/query_evaluator/Dummies/TestQueryBuilder.h"
#include "../../../unit_testing/src/query_evaluator/QETestUtils.h"
#include "catch.hpp"
#include "design_extractor/DesignExtractorManager.h"
#include "design_extractor/DesignExtractorRetriever.h"
#include "pkb/DataRetriever.h"
#include "pkb/PKBStorage.h"
#include "query_builder/clauses/such_that_clauses/ModifiesSClause.h"
#include "query_builder/clauses/such_that_clauses/UsesSClause.h"
#include "query_evaluator/QueryEvaluator.h"

TEST_CASE("Test de-pkb-qe integration") {
  const vector<string> FALSE_RESULT = {"FALSE"};
  const vector<string> TRUE_RESULT = {"TRUE"};
  const vector<string> EMPTY_RESULT = {};
  const int PROGRAM_NODE_IDX_OFFSET = 1;

  SECTION("variable v; select v from procedure 2") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    shared_ptr<DataRetriever> dataRetriever = make_shared<DataRetriever>(pkb);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(2 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();

    Synonym syn1 = Synonym("v");

    auto builder = make_shared<TestQE::TestQueryBuilder>();
    auto testQuery = builder->addDeclaration(QB::DesignEntity::VARIABLE, syn1)
                         ->addToSelect(syn1)
                         ->build();

    auto actual = queryEvaluator->evaluate(testQuery);
    vector<string> expected = {"x", "y"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }

  SECTION("variable v; select Boolean from procedure 2: true result") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    shared_ptr<DataRetriever> dataRetriever = make_shared<DataRetriever>(pkb);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(2 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();

    Synonym syn1 = Synonym("v");

    auto builder = make_shared<TestQE::TestQueryBuilder>();
    auto testQuery = builder->addDeclaration(QB::DesignEntity::VARIABLE, syn1)
                         ->setReturnBoolean()
                         ->build();

    auto actual = queryEvaluator->evaluate(testQuery);
    vector<string> expected = {"TRUE"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }

  SECTION(
      "variable v; select Boolean such that Modifies(2, 'x') from procedure "
      "false result") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    shared_ptr<DataRetriever> dataRetriever = make_shared<DataRetriever>(pkb);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(2 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();

    Synonym syn1 = Synonym("v");

    auto builder = make_shared<TestQE::TestQueryBuilder>();
    auto testQuery = builder->addDeclaration(QB::DesignEntity::VARIABLE, syn1)
                         ->setReturnBoolean()
                         ->addModifiesS(2, Ident("x"))
                         ->build();

    auto actual = queryEvaluator->evaluate(testQuery);
    vector<string> expected = {"FALSE"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }

  SECTION("variable v; select <v, v> from procedure 2") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    shared_ptr<DataRetriever> dataRetriever = make_shared<DataRetriever>(pkb);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(2 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();

    Synonym syn1 = Synonym("v");

    auto builder = make_shared<TestQE::TestQueryBuilder>();
    auto query = builder->addDeclaration(QB::DesignEntity::VARIABLE, syn1)
                     ->addToSelect(syn1)
                     ->addToSelect(syn1)
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"x x", "y y"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }

  SECTION("print pr; select pr with pr.varName = 'x'; from procedure 3") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    shared_ptr<DataRetriever> dataRetriever = make_shared<DataRetriever>(pkb);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(3 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();

    Synonym syn1 = Synonym("pr");

    auto query =
        make_shared<TestQE::TestQueryBuilder>()
            ->addDeclaration(QB::DesignEntity::PRINT, syn1)
            ->addToSelect(syn1)
            ->addWith(AttrRef(syn1, QB::AttrName::VAR_NAME), Ident("x"))
            ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"1"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }

  SECTION(
      "print pr; select pr.varName such that Uses(pr, 'x'); from procedure") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    shared_ptr<DataRetriever> dataRetriever = make_shared<DataRetriever>(pkb);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(3 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();

    Synonym syn1 = Synonym("pr");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::PRINT, syn1)
                     ->addToSelect(AttrRef(syn1, QB::AttrName::VAR_NAME))
                     ->addUsesS(syn1, Ident("x"))
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"x"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }

  SECTION(
      "print pr; read rd; select <rd, pr.varName> such that Uses(pr, 'x') and "
      "Modifies(rd, _); from procedure 3") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    shared_ptr<DataRetriever> dataRetriever = make_shared<DataRetriever>(pkb);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(3 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();

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

    // rd: 2 6 7
    // pr: 1
    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"2 x", "6 x", "7 x"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }

  SECTION(
      "print pr; read rd; variable v; select <rd, pr.varName, v> such that "
      "Uses(pr, _) and Modifies(rd, 'foo'); from procedure 3") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    shared_ptr<DataRetriever> dataRetriever = make_shared<DataRetriever>(pkb);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(3 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();

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

    // rd: 7
    // pr: 1 5
    // v: x y bar z w foo
    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"7 x x",   "7 x y",   "7 x bar", "7 x z",
                               "7 x w",   "7 x foo", "7 z x",   "7 z y",
                               "7 z bar", "7 z z",   "7 z w",   "7 z foo"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }

  SECTION("print pr; select pr with pr.varname == 'x'; from procedure 3") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    shared_ptr<DataRetriever> dataRetriever = make_shared<DataRetriever>(pkb);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(3 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();

    Synonym syn1 = Synonym("pr");

    auto query =
        make_shared<TestQE::TestQueryBuilder>()
            ->addDeclaration(QB::DesignEntity::PRINT, syn1)
            ->addToSelect(syn1)
            ->addWith(AttrRef(syn1, QB::AttrName::VAR_NAME), Ident("x"))
            ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"1"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }

  SECTION(
      "print pr; select pr.varname with pr.varname == 'x'; from procedure 3") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    shared_ptr<DataRetriever> dataRetriever = make_shared<DataRetriever>(pkb);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(3 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();

    Synonym syn1 = Synonym("pr");

    auto query =
        make_shared<TestQE::TestQueryBuilder>()
            ->addDeclaration(QB::DesignEntity::PRINT, syn1)
            ->addToSelect(AttrRef(syn1, QB::AttrName::VAR_NAME))
            ->addWith(AttrRef(syn1, QB::AttrName::VAR_NAME), Ident("x"))
            ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"x"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }

  // test such that + with
  SECTION(
      "variable v; print pr; if ifs; Select pr with v.varname = pr.varname and "
      "parent*(ifs, pr); from procedure 4") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    shared_ptr<DataRetriever> dataRetriever = make_shared<DataRetriever>(pkb);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(4 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();

    Synonym syn1 = Synonym("v");
    Synonym syn2 = Synonym("pr");
    Synonym syn3 = Synonym("ifs");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::VARIABLE, syn1)
                     ->addDeclaration(QB::DesignEntity::PRINT, syn2)
                     ->addDeclaration(QB::DesignEntity::IF, syn3)
                     ->addToSelect(syn2)
                     ->addWith(AttrRef(syn1, QB::AttrName::VAR_NAME),
                               AttrRef(syn2, QB::AttrName::VAR_NAME))
                     ->addParentT(syn3, syn2)
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"4", "7"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }

  SECTION(
      "variable v; print pr; if ifs; Select ifs with v.varname = pr.varname "
      "and parent(ifs, pr); from procedure 4") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    shared_ptr<DataRetriever> dataRetriever = make_shared<DataRetriever>(pkb);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(4 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();

    Synonym syn1 = Synonym("v");
    Synonym syn2 = Synonym("pr");
    Synonym syn3 = Synonym("ifs");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::VARIABLE, syn1)
                     ->addDeclaration(QB::DesignEntity::PRINT, syn2)
                     ->addDeclaration(QB::DesignEntity::IF, syn3)
                     ->addToSelect(syn3)
                     ->addWith(AttrRef(syn1, QB::AttrName::VAR_NAME),
                               AttrRef(syn2, QB::AttrName::VAR_NAME))
                     ->addParent(syn3, syn2)
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"3"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }

  // test with clause with constant value
  SECTION(
      "constant c; stmt s;  Select s with c.value = s.stmt# ; from procedure "
      "4") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    shared_ptr<DataRetriever> dataRetriever = make_shared<DataRetriever>(pkb);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(4 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();

    Synonym syn1 = Synonym("c");
    Synonym syn2 = Synonym("s");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::CONSTANT, syn1)
                     ->addDeclaration(QB::DesignEntity::STMT, syn2)
                     ->addToSelect(syn2)
                     ->addWith(AttrRef(syn1, QB::AttrName::VALUE),
                               AttrRef(syn2, QB::AttrName::STMT_NUMBER))
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"3", "1", "5"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }

  // test with clause + such that clause with constant value
  SECTION(
      "constant c; stmt s;  Select s with c.value = s.stmt# and such that "
      "parent(s, 4); from procedure 4") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    shared_ptr<DataRetriever> dataRetriever = make_shared<DataRetriever>(pkb);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(4 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();

    Synonym syn1 = Synonym("c");
    Synonym syn2 = Synonym("s");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::CONSTANT, syn1)
                     ->addDeclaration(QB::DesignEntity::STMT, syn2)
                     ->addToSelect(syn2)
                     ->addParent(syn2, 4)
                     ->addWith(AttrRef(syn1, QB::AttrName::VALUE),
                               AttrRef(syn2, QB::AttrName::STMT_NUMBER))
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"3"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }

  SECTION(
      "print pr; select pr.varName such that Next(pr, 2); from procedure 3") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    shared_ptr<DataRetriever> dataRetriever = make_shared<DataRetriever>(pkb);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(3 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();

    Synonym syn1 = Synonym("pr");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::PRINT, syn1)
                     ->addToSelect(AttrRef(syn1, QB::AttrName::VAR_NAME))
                     ->addNext(syn1, 2)
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"x"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }

  SECTION("if ifs; Select ifs pattern('bar',_, _ ) from procedure 3") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    shared_ptr<DataRetriever> dataRetriever = make_shared<DataRetriever>(pkb);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(3 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();

    Synonym syn1 = Synonym("ifs");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::IF, syn1)
                     ->addToSelect(syn1)
                     ->addIfPattern(syn1, Ident("bar"))
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"3"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }

  SECTION("while w; Select w pattern('qux',_, ) from procedure 4") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    shared_ptr<DataRetriever> dataRetriever = make_shared<DataRetriever>(pkb);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(4 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();

    Synonym syn1 = Synonym("w");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::WHILE, syn1)
                     ->addToSelect(syn1)
                     ->addWhilePattern(syn1, Ident("qux"))
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"5"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }

  SECTION("stmt s; select s such that Follows(1, s) from procedure 4") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    shared_ptr<DataRetriever> dataRetriever = make_shared<DataRetriever>(pkb);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(4 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();

    Synonym syn1 = Synonym("s");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::STMT, syn1)
                     ->addToSelect(syn1)
                     ->addFollows(1, syn1)
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"2"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }
  SECTION("stmt s; select s such that Follows*(s, 2) from procedure 4") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    shared_ptr<DataRetriever> dataRetriever = make_shared<DataRetriever>(pkb);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(4 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();

    Synonym syn1 = Synonym("s");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::STMT, syn1)
                     ->addToSelect(syn1)
                     ->addFollowsT(syn1, 2)
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"1"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }
  SECTION(
      "variable v; select v such that usesP('procedure3', v) and uses(4, v) "
      "from procedure 4") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    shared_ptr<DataRetriever> dataRetriever = make_shared<DataRetriever>(pkb);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(4 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();

    Synonym syn1 = Synonym("v");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::VARIABLE, syn1)
                     ->addToSelect(syn1)
                     ->addUsesP(Ident("procedure3"), syn1)
                     ->addUsesS(4, syn1)
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"z"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }
  SECTION(
      "procedure p; select p such that modifiesP(p, 'baz')  from procedure 4") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    shared_ptr<DataRetriever> dataRetriever = make_shared<DataRetriever>(pkb);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(4 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();

    Synonym syn1 = Synonym("p");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::PROCEDURE, syn1)
                     ->addToSelect(syn1)
                     ->addModifiesP(syn1, Ident("baz"))
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"procedure3"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }
  SECTION(
      "procedure p, p1; select p such that calls(p, _)  from procedure 11") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    shared_ptr<DataRetriever> dataRetriever = make_shared<DataRetriever>(pkb);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(11 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();

    Synonym syn1 = Synonym("p");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::PROCEDURE, syn1)
                     ->addToSelect(syn1)
                     ->addCalls(syn1, Underscore())
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"procedure2"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }
  SECTION(
      "procedure p, p1; select p such that calls*(p, _)  from procedure 11") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    shared_ptr<DataRetriever> dataRetriever = make_shared<DataRetriever>(pkb);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(11 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();

    Synonym syn1 = Synonym("p");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::PROCEDURE, syn1)
                     ->addToSelect(syn1)
                     ->addCallT(syn1, Underscore())
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"procedure2"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }
  SECTION("stmt s; select s such that next(1, s)  from procedure 12") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    shared_ptr<DataRetriever> dataRetriever = make_shared<DataRetriever>(pkb);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(12 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();

    Synonym syn1 = Synonym("s");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::STMT, syn1)
                     ->addToSelect(syn1)
                     ->addNext(1, syn1)
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"2"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }

  SECTION("stmt s; select s such that next*(1, s)  from procedure 12") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(12 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();
    shared_ptr<DE::DesignExtractorRetriever> designExtractorRetriever =
        make_shared<DE::DesignExtractorRetriever>(pNode);
    shared_ptr<CacheManager> cacheManager =
        make_shared<CacheManager>(CacheManager(designExtractorRetriever));
    shared_ptr<DataRetriever> dataRetriever =
        make_shared<DataRetriever>(pkb, cacheManager);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);

    Synonym syn1 = Synonym("s");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::STMT, syn1)
                     ->addToSelect(syn1)
                     ->addNextT(1, syn1)
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"2", "3", "4"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }
  SECTION("stmt s; select s such that affect(1, s)  from procedure 12") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(12 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();
    shared_ptr<DE::DesignExtractorRetriever> designExtractorRetriever =
        make_shared<DE::DesignExtractorRetriever>(pNode);
    shared_ptr<CacheManager> cacheManager =
        make_shared<CacheManager>(CacheManager(designExtractorRetriever));
    shared_ptr<DataRetriever> dataRetriever =
        make_shared<DataRetriever>(pkb, cacheManager);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);

    Synonym syn1 = Synonym("s");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::STMT, syn1)
                     ->addToSelect(syn1)
                     ->addAffects(1, syn1)
                     ->build();

    queryEvaluator->evaluate(query);
    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }
    SECTION("stmt s; select s such that affect(1000, s)  from procedure 12: out of range") {
        shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
        auto pNode =
                TestDE::Dummies::getTestProgramNode(12 - PROGRAM_NODE_IDX_OFFSET);
        shared_ptr<DE::DesignExtractorManager> designExtractorManager =
                make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
        designExtractorManager->run();
        shared_ptr<DE::DesignExtractorRetriever> designExtractorRetriever =
                make_shared<DE::DesignExtractorRetriever>(pNode);
        shared_ptr<CacheManager> cacheManager =
                make_shared<CacheManager>(CacheManager(designExtractorRetriever));
        shared_ptr<DataRetriever> dataRetriever =
                make_shared<DataRetriever>(pkb, cacheManager);
        shared_ptr<QE::QueryEvaluator> queryEvaluator =
                make_shared<QE::QueryEvaluator>(dataRetriever);

        Synonym syn1 = Synonym("s");

        auto query = make_shared<TestQE::TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addToSelect(syn1)
                ->addAffects(1000, syn1)
                ->build();

        auto actual = queryEvaluator->evaluate(query);
        vector<string> expected = {};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("stmt s; select s such that affect(s, 1000)  from procedure 12: out of range") {
        shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
        auto pNode =
                TestDE::Dummies::getTestProgramNode(12 - PROGRAM_NODE_IDX_OFFSET);
        shared_ptr<DE::DesignExtractorManager> designExtractorManager =
                make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
        designExtractorManager->run();
        shared_ptr<DE::DesignExtractorRetriever> designExtractorRetriever =
                make_shared<DE::DesignExtractorRetriever>(pNode);
        shared_ptr<CacheManager> cacheManager =
                make_shared<CacheManager>(CacheManager(designExtractorRetriever));
        shared_ptr<DataRetriever> dataRetriever =
                make_shared<DataRetriever>(pkb, cacheManager);
        shared_ptr<QE::QueryEvaluator> queryEvaluator =
                make_shared<QE::QueryEvaluator>(dataRetriever);

        Synonym syn1 = Synonym("s");

        auto query = make_shared<TestQE::TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addToSelect(syn1)
                ->addAffects( syn1, 1000)
                ->build();

        auto actual = queryEvaluator->evaluate(query);
        vector<string> expected = {};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("stmt s; select s such that affect(s, 9)  from procedure 12: in range boundary value") {
        shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
        auto pNode =
                TestDE::Dummies::getTestProgramNode(12 - PROGRAM_NODE_IDX_OFFSET);
        shared_ptr<DE::DesignExtractorManager> designExtractorManager =
                make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
        designExtractorManager->run();
        shared_ptr<DE::DesignExtractorRetriever> designExtractorRetriever =
                make_shared<DE::DesignExtractorRetriever>(pNode);
        shared_ptr<CacheManager> cacheManager =
                make_shared<CacheManager>(CacheManager(designExtractorRetriever));
        shared_ptr<DataRetriever> dataRetriever =
                make_shared<DataRetriever>(pkb, cacheManager);
        shared_ptr<QE::QueryEvaluator> queryEvaluator =
                make_shared<QE::QueryEvaluator>(dataRetriever);

        Synonym syn1 = Synonym("s");

        auto query = make_shared<TestQE::TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addToSelect(syn1)
                ->addAffects( syn1, 9)
                ->build();

        auto actual = queryEvaluator->evaluate(query);
        vector<string> expected = {};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("stmt s; select s such that affect(s, 10)  from procedure 12: out of range boundary value") {
        shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
        auto pNode =
                TestDE::Dummies::getTestProgramNode(12 - PROGRAM_NODE_IDX_OFFSET);
        shared_ptr<DE::DesignExtractorManager> designExtractorManager =
                make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
        designExtractorManager->run();
        shared_ptr<DE::DesignExtractorRetriever> designExtractorRetriever =
                make_shared<DE::DesignExtractorRetriever>(pNode);
        shared_ptr<CacheManager> cacheManager =
                make_shared<CacheManager>(CacheManager(designExtractorRetriever));
        shared_ptr<DataRetriever> dataRetriever =
                make_shared<DataRetriever>(pkb, cacheManager);
        shared_ptr<QE::QueryEvaluator> queryEvaluator =
                make_shared<QE::QueryEvaluator>(dataRetriever);

        Synonym syn1 = Synonym("s");

        auto query = make_shared<TestQE::TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addToSelect(syn1)
                ->addAffects( syn1, 10)
                ->build();

        auto actual = queryEvaluator->evaluate(query);
        vector<string> expected = {};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("stmt s; select s such that affect(s, 10)  from procedure 12: out of range boundary value") {
        shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
        auto pNode =
                TestDE::Dummies::getTestProgramNode(12 - PROGRAM_NODE_IDX_OFFSET);
        shared_ptr<DE::DesignExtractorManager> designExtractorManager =
                make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
        designExtractorManager->run();
        shared_ptr<DE::DesignExtractorRetriever> designExtractorRetriever =
                make_shared<DE::DesignExtractorRetriever>(pNode);
        shared_ptr<CacheManager> cacheManager =
                make_shared<CacheManager>(CacheManager(designExtractorRetriever));
        shared_ptr<DataRetriever> dataRetriever =
                make_shared<DataRetriever>(pkb, cacheManager);
        shared_ptr<QE::QueryEvaluator> queryEvaluator =
                make_shared<QE::QueryEvaluator>(dataRetriever);

        Synonym syn1 = Synonym("s");

        auto query = make_shared<TestQE::TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addToSelect(syn1)
                ->addAffects(  10, syn1)
                ->build();

        auto actual = queryEvaluator->evaluate(query);
        vector<string> expected = {};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("stmt s; select s such that affect(s, 10)  from procedure 12: in range boundary value") {
        shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
        auto pNode =
                TestDE::Dummies::getTestProgramNode(12 - PROGRAM_NODE_IDX_OFFSET);
        shared_ptr<DE::DesignExtractorManager> designExtractorManager =
                make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
        designExtractorManager->run();
        shared_ptr<DE::DesignExtractorRetriever> designExtractorRetriever =
                make_shared<DE::DesignExtractorRetriever>(pNode);
        shared_ptr<CacheManager> cacheManager =
                make_shared<CacheManager>(CacheManager(designExtractorRetriever));
        shared_ptr<DataRetriever> dataRetriever =
                make_shared<DataRetriever>(pkb, cacheManager);
        shared_ptr<QE::QueryEvaluator> queryEvaluator =
                make_shared<QE::QueryEvaluator>(dataRetriever);

        Synonym syn1 = Synonym("s");

        auto query = make_shared<TestQE::TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addToSelect(syn1)
                ->addAffects( syn1, 9)
                ->build();

        auto actual = queryEvaluator->evaluate(query);
        vector<string> expected = {};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
    SECTION("stmt s; select s such that affect(1000, 1000)  from procedure 12: in range boundary value") {
        shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
        auto pNode =
                TestDE::Dummies::getTestProgramNode(12 - PROGRAM_NODE_IDX_OFFSET);
        shared_ptr<DE::DesignExtractorManager> designExtractorManager =
                make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
        designExtractorManager->run();
        shared_ptr<DE::DesignExtractorRetriever> designExtractorRetriever =
                make_shared<DE::DesignExtractorRetriever>(pNode);
        shared_ptr<CacheManager> cacheManager =
                make_shared<CacheManager>(CacheManager(designExtractorRetriever));
        shared_ptr<DataRetriever> dataRetriever =
                make_shared<DataRetriever>(pkb, cacheManager);
        shared_ptr<QE::QueryEvaluator> queryEvaluator =
                make_shared<QE::QueryEvaluator>(dataRetriever);

        Synonym syn1 = Synonym("s");

        auto query = make_shared<TestQE::TestQueryBuilder>()
                ->addDeclaration(QB::DesignEntity::STMT, syn1)
                ->addToSelect(syn1)
                ->addAffects( 1000, 1000)
                ->build();

        auto actual = queryEvaluator->evaluate(query);
        vector<string> expected = {};
        REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
    }
  SECTION("stmt s; select s such that affect*(1, s)  from procedure 15") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(15 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();
    shared_ptr<DE::DesignExtractorRetriever> designExtractorRetriever =
        make_shared<DE::DesignExtractorRetriever>(pNode);
    shared_ptr<CacheManager> cacheManager =
        make_shared<CacheManager>(CacheManager(designExtractorRetriever));
    shared_ptr<DataRetriever> dataRetriever =
        make_shared<DataRetriever>(pkb, cacheManager);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);

    Synonym syn1 = Synonym("s");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::STMT, syn1)
                     ->addToSelect(syn1)
                     ->addAffectsT(3, syn1)
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"4"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }
  SECTION("assign a; select a pattern a('x', '0')  from procedure 15") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(15 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();
    shared_ptr<DE::DesignExtractorRetriever> designExtractorRetriever =
        make_shared<DE::DesignExtractorRetriever>(pNode);
    shared_ptr<CacheManager> cacheManager =
        make_shared<CacheManager>(CacheManager(designExtractorRetriever));
    shared_ptr<DataRetriever> dataRetriever =
        make_shared<DataRetriever>(pkb, cacheManager);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);

    Synonym syn1 = Synonym("a");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::ASSIGN, syn1)
                     ->addToSelect(syn1)
                     ->addAssignPattern(
                         syn1, Ident("x"),
                         ExpressionSpec(QB::ExpressionSpecType::FULL_MATCH,
                                        make_shared<ExprNode>("0")))
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"1"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }
  SECTION("assign a; select boolean pattern a('x', '0')  from procedure 15") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(15 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();
    shared_ptr<DE::DesignExtractorRetriever> designExtractorRetriever =
        make_shared<DE::DesignExtractorRetriever>(pNode);
    shared_ptr<CacheManager> cacheManager =
        make_shared<CacheManager>(CacheManager(designExtractorRetriever));
    shared_ptr<DataRetriever> dataRetriever =
        make_shared<DataRetriever>(pkb, cacheManager);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);

    Synonym syn1 = Synonym("a");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::ASSIGN, syn1)
                     ->setReturnBoolean()
                     ->addAssignPattern(
                         syn1, Ident("x"),
                         ExpressionSpec(QB::ExpressionSpecType::FULL_MATCH,
                                        make_shared<ExprNode>("0")))
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    const vector<string>& expected = TRUE_RESULT;
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }
  SECTION(
      "assign a; select boolean pattern a('x', '0') and with 1 == 2  from "
      "procedure 15") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(15 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();
    shared_ptr<DE::DesignExtractorRetriever> designExtractorRetriever =
        make_shared<DE::DesignExtractorRetriever>(pNode);
    shared_ptr<CacheManager> cacheManager =
        make_shared<CacheManager>(CacheManager(designExtractorRetriever));
    shared_ptr<DataRetriever> dataRetriever =
        make_shared<DataRetriever>(pkb, cacheManager);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);

    Synonym syn1 = Synonym("a");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::ASSIGN, syn1)
                     ->setReturnBoolean()
                     ->addAssignPattern(
                         syn1, Ident("x"),
                         ExpressionSpec(QB::ExpressionSpecType::FULL_MATCH,
                                        make_shared<ExprNode>("0")))
                     ->addWith(1, 2)
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    const vector<string>& expected = FALSE_RESULT;
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }
  SECTION(
      "print pr; select boolean  with pr.varname = 'x'  from procedure 12") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(12 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();
    shared_ptr<DE::DesignExtractorRetriever> designExtractorRetriever =
        make_shared<DE::DesignExtractorRetriever>(pNode);
    shared_ptr<CacheManager> cacheManager =
        make_shared<CacheManager>(CacheManager(designExtractorRetriever));
    shared_ptr<DataRetriever> dataRetriever =
        make_shared<DataRetriever>(pkb, cacheManager);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);

    Synonym syn1 = Synonym("pr");

    auto query =
        make_shared<TestQE::TestQueryBuilder>()
            ->addDeclaration(QB::DesignEntity::PRINT, syn1)
            ->setReturnBoolean()
            ->addWith(AttrRef(syn1, QB::AttrName::VAR_NAME), Ident("x"))
            ->build();

    auto actual = queryEvaluator->evaluate(query);
    const vector<string>& expected = TRUE_RESULT;
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }
  SECTION("print pr; select pr.varname from procedure 12") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(12 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();
    shared_ptr<DE::DesignExtractorRetriever> designExtractorRetriever =
        make_shared<DE::DesignExtractorRetriever>(pNode);
    shared_ptr<CacheManager> cacheManager =
        make_shared<CacheManager>(CacheManager(designExtractorRetriever));
    shared_ptr<DataRetriever> dataRetriever =
        make_shared<DataRetriever>(pkb, cacheManager);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);

    Synonym syn1 = Synonym("pr");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::PRINT, syn1)
                     ->addToSelect(AttrRef(syn1, QB::AttrName::VAR_NAME))
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"x", "y", "z"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }
  SECTION(
      "print pr; read rd; select pr.varname with rd.varname == adda from "
      "procedure 12") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(12 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();
    shared_ptr<DE::DesignExtractorRetriever> designExtractorRetriever =
        make_shared<DE::DesignExtractorRetriever>(pNode);
    shared_ptr<CacheManager> cacheManager =
        make_shared<CacheManager>(CacheManager(designExtractorRetriever));
    shared_ptr<DataRetriever> dataRetriever =
        make_shared<DataRetriever>(pkb, cacheManager);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);

    Synonym syn1 = Synonym("pr");
    Synonym syn2 = Synonym("rd");

    auto query =
        make_shared<TestQE::TestQueryBuilder>()
            ->addDeclaration(QB::DesignEntity::PRINT, syn1)
            ->addDeclaration(QB::DesignEntity::READ, syn2)
            ->addToSelect(AttrRef(syn1, QB::AttrName::VAR_NAME))
            ->addWith(AttrRef(syn2, QB::AttrName::VAR_NAME), Ident("adda"))
            ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"x", "y", "z"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }
  SECTION(
      "print pr; read rd; select pr.varname with rd.varname == adda and "
      "rd.stmt# == 6 from procedure 12") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(12 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();
    shared_ptr<DE::DesignExtractorRetriever> designExtractorRetriever =
        make_shared<DE::DesignExtractorRetriever>(pNode);
    shared_ptr<CacheManager> cacheManager =
        make_shared<CacheManager>(CacheManager(designExtractorRetriever));
    shared_ptr<DataRetriever> dataRetriever =
        make_shared<DataRetriever>(pkb, cacheManager);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);

    Synonym syn1 = Synonym("pr");
    Synonym syn2 = Synonym("rd");

    auto query =
        make_shared<TestQE::TestQueryBuilder>()
            ->addDeclaration(QB::DesignEntity::PRINT, syn1)
            ->addDeclaration(QB::DesignEntity::READ, syn2)
            ->addToSelect(AttrRef(syn1, QB::AttrName::VAR_NAME))
            ->addWith(AttrRef(syn2, QB::AttrName::VAR_NAME), Ident("adda"))
            ->addWith(AttrRef(syn2, QB::AttrName::STMT_NUMBER), 6)
            ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"x", "y", "z"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }
  SECTION(
      "print pr; read rd; select pr.varname with rd.varname == adda and "
      "rd.stmt# == 6 from procedure 12") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(12 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();
    shared_ptr<DE::DesignExtractorRetriever> designExtractorRetriever =
        make_shared<DE::DesignExtractorRetriever>(pNode);
    shared_ptr<CacheManager> cacheManager =
        make_shared<CacheManager>(CacheManager(designExtractorRetriever));
    shared_ptr<DataRetriever> dataRetriever =
        make_shared<DataRetriever>(pkb, cacheManager);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);

    Synonym syn1 = Synonym("pr");
    Synonym syn2 = Synonym("rd");

    auto query =
        make_shared<TestQE::TestQueryBuilder>()
            ->addDeclaration(QB::DesignEntity::PRINT, syn1)
            ->addDeclaration(QB::DesignEntity::READ, syn2)
            ->addToSelect(AttrRef(syn1, QB::AttrName::VAR_NAME))
            ->addWith(AttrRef(syn2, QB::AttrName::VAR_NAME), Ident("adda"))
            ->addWith(6, AttrRef(syn2, QB::AttrName::STMT_NUMBER))
            ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"x", "y", "z"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }
  SECTION(
      "print pr; read rd; select pr.varname with rd.varname == pr.varname from "
      "procedure 12") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(12 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();
    shared_ptr<DE::DesignExtractorRetriever> designExtractorRetriever =
        make_shared<DE::DesignExtractorRetriever>(pNode);
    shared_ptr<CacheManager> cacheManager =
        make_shared<CacheManager>(CacheManager(designExtractorRetriever));
    shared_ptr<DataRetriever> dataRetriever =
        make_shared<DataRetriever>(pkb, cacheManager);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);

    Synonym syn1 = Synonym("pr");
    Synonym syn2 = Synonym("rd");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::PRINT, syn1)
                     ->addDeclaration(QB::DesignEntity::READ, syn2)
                     ->addToSelect(AttrRef(syn1, QB::AttrName::VAR_NAME))
                     ->addWith(AttrRef(syn2, QB::AttrName::VAR_NAME),
                               AttrRef(syn1, QB::AttrName::VAR_NAME))
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }
  SECTION(
      "print pr; read rd; select pr.varname with 'adda' == pr.varname from "
      "procedure 12") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(12 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();
    shared_ptr<DE::DesignExtractorRetriever> designExtractorRetriever =
        make_shared<DE::DesignExtractorRetriever>(pNode);
    shared_ptr<CacheManager> cacheManager =
        make_shared<CacheManager>(CacheManager(designExtractorRetriever));
    shared_ptr<DataRetriever> dataRetriever =
        make_shared<DataRetriever>(pkb, cacheManager);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);

    Synonym syn1 = Synonym("pr");
    Synonym syn2 = Synonym("rd");

    auto query =
        make_shared<TestQE::TestQueryBuilder>()
            ->addDeclaration(QB::DesignEntity::PRINT, syn1)
            ->addDeclaration(QB::DesignEntity::READ, syn2)
            ->addToSelect(AttrRef(syn1, QB::AttrName::VAR_NAME))
            ->addWith(Ident("adda"), AttrRef(syn1, QB::AttrName::VAR_NAME))
            ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }
  SECTION(
      "print pr; read rd; select pr.varname with adda = adda from procedure "
      "12") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(12 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();
    shared_ptr<DE::DesignExtractorRetriever> designExtractorRetriever =
        make_shared<DE::DesignExtractorRetriever>(pNode);
    shared_ptr<CacheManager> cacheManager =
        make_shared<CacheManager>(CacheManager(designExtractorRetriever));
    shared_ptr<DataRetriever> dataRetriever =
        make_shared<DataRetriever>(pkb, cacheManager);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);

    Synonym syn1 = Synonym("pr");
    Synonym syn2 = Synonym("rd");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::PRINT, syn1)
                     ->addDeclaration(QB::DesignEntity::READ, syn2)
                     ->addToSelect(AttrRef(syn1, QB::AttrName::VAR_NAME))
                     ->addWith(Ident("adda"), Ident("adda"))
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"x", "y", "z"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }
  SECTION(
      "print pr; read rd; select pr.stmt# with adda = adda from procedure 12") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(12 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();
    shared_ptr<DE::DesignExtractorRetriever> designExtractorRetriever =
        make_shared<DE::DesignExtractorRetriever>(pNode);
    shared_ptr<CacheManager> cacheManager =
        make_shared<CacheManager>(CacheManager(designExtractorRetriever));
    shared_ptr<DataRetriever> dataRetriever =
        make_shared<DataRetriever>(pkb, cacheManager);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);

    Synonym syn1 = Synonym("pr");
    Synonym syn2 = Synonym("rd");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::PRINT, syn1)
                     ->addDeclaration(QB::DesignEntity::READ, syn2)
                     ->addToSelect(AttrRef(syn1, QB::AttrName::STMT_NUMBER))
                     ->addWith(Ident("adda"), Ident("adda"))
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"1", "5", "8"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }
  SECTION("print pr; read rd; select pr.stmt#  from procedure 12") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(12 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();
    shared_ptr<DE::DesignExtractorRetriever> designExtractorRetriever =
        make_shared<DE::DesignExtractorRetriever>(pNode);
    shared_ptr<CacheManager> cacheManager =
        make_shared<CacheManager>(CacheManager(designExtractorRetriever));
    shared_ptr<DataRetriever> dataRetriever =
        make_shared<DataRetriever>(pkb, cacheManager);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);

    Synonym syn1 = Synonym("pr");
    Synonym syn2 = Synonym("rd");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::PRINT, syn1)
                     ->addDeclaration(QB::DesignEntity::READ, syn2)
                     ->addToSelect(AttrRef(syn1, QB::AttrName::STMT_NUMBER))
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"1", "5", "8"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }
  SECTION(
      "procedure p; select p.procname with p.procname = 'procedure2' from "
      "procedure 12") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(12 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();
    shared_ptr<DE::DesignExtractorRetriever> designExtractorRetriever =
        make_shared<DE::DesignExtractorRetriever>(pNode);
    shared_ptr<CacheManager> cacheManager =
        make_shared<CacheManager>(CacheManager(designExtractorRetriever));
    shared_ptr<DataRetriever> dataRetriever =
        make_shared<DataRetriever>(pkb, cacheManager);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);

    Synonym syn1 = Synonym("p");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::PROCEDURE, syn1)
                     ->addToSelect(AttrRef(syn1, QB::AttrName::PROC_NAME))
                     ->addWith(AttrRef(syn1, QB::AttrName::PROC_NAME),
                               Ident("procedure2"))
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"procedure2"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }
  SECTION(
      "call cl; select cl with cl.procname = 'procedure4' from procedure 12") {
    shared_ptr<PKBStorage> pkb = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkb);
    auto pNode =
        TestDE::Dummies::getTestProgramNode(12 - PROGRAM_NODE_IDX_OFFSET);
    shared_ptr<DE::DesignExtractorManager> designExtractorManager =
        make_shared<DE::DesignExtractorManager>(dataModifier, pNode);
    designExtractorManager->run();
    shared_ptr<DE::DesignExtractorRetriever> designExtractorRetriever =
        make_shared<DE::DesignExtractorRetriever>(pNode);
    shared_ptr<CacheManager> cacheManager =
        make_shared<CacheManager>(CacheManager(designExtractorRetriever));
    shared_ptr<DataRetriever> dataRetriever =
        make_shared<DataRetriever>(pkb, cacheManager);
    shared_ptr<QE::QueryEvaluator> queryEvaluator =
        make_shared<QE::QueryEvaluator>(dataRetriever);

    Synonym syn1 = Synonym("cl");

    auto query = make_shared<TestQE::TestQueryBuilder>()
                     ->addDeclaration(QB::DesignEntity::CALL, syn1)
                     ->addToSelect(syn1)
                     ->addWith(AttrRef(syn1, QB::AttrName::PROC_NAME),
                               Ident("procedure4"))
                     ->build();

    auto actual = queryEvaluator->evaluate(query);
    vector<string> expected = {"3", "7"};
    REQUIRE(QETest::QETestUtils::containsSameElement(actual, expected));
  }
}