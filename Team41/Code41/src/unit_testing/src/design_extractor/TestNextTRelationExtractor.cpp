//
// Created by Xingchen Lin on 27/10/22.
//

#include <unordered_map>

#include "AST/IfNode.h"
#include "AST/PrintNode.h"
#include "DEUtils.h"
#include "Dummies.h"
#include "catch.hpp"
#include "design_extractor/extractors/NextTRelationExtractor.h"
#include "pkb/DataModifier.h"

using namespace std;
using namespace DE;

TEST_CASE("Test NextT Relation Extractor") {
  SECTION("Test No Arguments In Simple Procedure") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(1);
    NextTRelationExtractor nextTExtractor = NextTRelationExtractor(programNode);
    auto actual = static_pointer_cast<RelationResult>(
                      nextTExtractor.extractAllRelations())
                      ->getResult();
    vector<vector<string>> expected = {{"1", "2"}, {"2", "3"}, {"1", "3"}};
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("Test No Arguments In Normal Procedure") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(2);
    NextTRelationExtractor nextTExtractor = NextTRelationExtractor(programNode);
    auto actual = static_pointer_cast<RelationResult>(
                      nextTExtractor.extractAllRelations())
                      ->getResult();
    vector<vector<string>> expected = {
        {"1", "2"}, {"1", "3"}, {"1", "4"}, {"1", "5"}, {"1", "6"},
        {"1", "7"}, {"2", "3"}, {"2", "4"}, {"2", "5"}, {"2", "6"},
        {"2", "7"}, {"3", "4"}, {"3", "5"}, {"3", "6"}, {"3", "7"},
        {"4", "5"}, {"4", "7"}, {"5", "7"}, {"6", "7"}};
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("Test No Arguments With While Statement") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(24);
    NextTRelationExtractor nextTExtractor = NextTRelationExtractor(programNode);
    auto actual = static_pointer_cast<RelationResult>(
                      nextTExtractor.extractAllRelations())
                      ->getResult();
    vector<vector<string>> expected = {{"1", "2"}, {"1", "3"}, {"1", "4"},
                                       {"2", "2"}, {"2", "3"}, {"2", "4"},
                                       {"3", "2"}, {"3", "3"}, {"3", "4"}};
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("Test Invalid Start and End Arguments") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(1);
    StmtNoArgs args = StmtNoArgs();
    args.setStartStmtNo(100);
    args.setEndStmtNo(250);
    NextTRelationExtractor nextTExtractor = NextTRelationExtractor(programNode);
    auto actual =
        static_pointer_cast<QueryTimeResult>(nextTExtractor.extract(args))
            ->getResult();
    REQUIRE(actual->empty());
  }

  SECTION("Test Invalid Start and Valid End Arguments") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(1);
    StmtNoArgs args = StmtNoArgs();
    args.setStartStmtNo(-100);
    args.setEndStmtNo(2);
    NextTRelationExtractor nextTExtractor = NextTRelationExtractor(programNode);
    auto actual =
        static_pointer_cast<QueryTimeResult>(nextTExtractor.extract(args))
            ->getResult();
    REQUIRE(actual->empty());
  }

  SECTION("Test Valid Start and Invalid End Arguments") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(1);
    StmtNoArgs args = StmtNoArgs();
    args.setStartStmtNo(1);
    args.setEndStmtNo(2000);
    NextTRelationExtractor nextTExtractor = NextTRelationExtractor(programNode);
    auto actual =
        static_pointer_cast<QueryTimeResult>(nextTExtractor.extract(args))
            ->getResult();
    REQUIRE(actual->empty());
  }

  SECTION("Test Invalid End Arguments") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(10);
    StmtNoArgs args = StmtNoArgs();
    args.setEndStmtNo(-4000);
    NextTRelationExtractor nextTExtractor = NextTRelationExtractor(programNode);
    auto actual =
        static_pointer_cast<QueryTimeResult>(nextTExtractor.extract(args))
            ->getResult();
    vector<string> expected = {};
    REQUIRE(*actual == expected);
  }

  SECTION("Test Valid Start and End Arguments But Different Procedures") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(10);
    StmtNoArgs args = StmtNoArgs();
    args.setStartStmtNo(3);
    args.setEndStmtNo(4);
    NextTRelationExtractor nextTExtractor = NextTRelationExtractor(programNode);
    auto actual =
        static_pointer_cast<QueryTimeResult>(nextTExtractor.extract(args))
            ->getResult();
    vector<string> expected = {};
    REQUIRE(*actual == expected);
  }

  SECTION("Test Valid Start and End Arguments") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(2);
    StmtNoArgs args = StmtNoArgs();
    args.setStartStmtNo(1);
    args.setEndStmtNo(6);
    NextTRelationExtractor nextTExtractor = NextTRelationExtractor(programNode);
    auto actual =
        static_pointer_cast<QueryTimeResult>(nextTExtractor.extract(args))
            ->getResult();
    vector<string> expected = {"1", "6"};
    REQUIRE(*actual == expected);
  }

  SECTION("Test Valid Start and End Arguments In While Statement") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(3);
    StmtNoArgs args = StmtNoArgs();
    args.setStartStmtNo(5);
    args.setEndStmtNo(5);
    NextTRelationExtractor nextTExtractor = NextTRelationExtractor(programNode);
    auto actual =
        static_pointer_cast<QueryTimeResult>(nextTExtractor.extract(args))
            ->getResult();
    vector<string> expected = {"5", "5"};
    REQUIRE(*actual == expected);
  }

  SECTION("Test Valid Start Only Arguments In If Statement") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(2);
    StmtNoArgs args = StmtNoArgs();
    args.setStartStmtNo(1);
    NextTRelationExtractor nextTExtractor = NextTRelationExtractor(programNode);
    auto actual =
        static_pointer_cast<QueryTimeResult>(nextTExtractor.extract(args))
            ->getResult();
    vector<string> expected = {"2", "3", "4", "5", "6", "7"};
    REQUIRE(
        TestDE::DEUtils::vectorStringContainsSameElements(*actual, expected));
  }

  SECTION("Test Valid Start Only Arguments In While Statement") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(3);
    StmtNoArgs args = StmtNoArgs();
    args.setStartStmtNo(5);
    NextTRelationExtractor nextTExtractor = NextTRelationExtractor(programNode);
    auto actual =
        static_pointer_cast<QueryTimeResult>(nextTExtractor.extract(args))
            ->getResult();
    vector<string> expected = {"5", "6", "7", "9"};
    REQUIRE(
        TestDE::DEUtils::vectorStringContainsSameElements(*actual, expected));
  }

  SECTION(
      "Test Valid Start Only Arguments In While Statement In Another "
      "Procedure") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(13);
    StmtNoArgs args = StmtNoArgs();
    args.setStartStmtNo(8);
    NextTRelationExtractor nextTExtractor = NextTRelationExtractor(programNode);
    auto actual =
        static_pointer_cast<QueryTimeResult>(nextTExtractor.extract(args))
            ->getResult();
    vector<string> expected = {"8", "9", "10"};
    REQUIRE(
        TestDE::DEUtils::vectorStringContainsSameElements(*actual, expected));
  }

  SECTION("Test Valid End Only Arguments Through Double Nested Statements") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(5);
    StmtNoArgs args = StmtNoArgs();
    args.setEndStmtNo(7);
    NextTRelationExtractor nextTExtractor = NextTRelationExtractor(programNode);
    auto actual =
        static_pointer_cast<QueryTimeResult>(nextTExtractor.extract(args))
            ->getResult();
    vector<string> expected = {"1", "2", "3", "4", "5", "6"};
    REQUIRE(
        TestDE::DEUtils::vectorStringContainsSameElements(*actual, expected));
  }

  SECTION("Test Valid End Only Arguments In If Statement") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(2);
    StmtNoArgs args = StmtNoArgs();
    args.setEndStmtNo(6);
    NextTRelationExtractor nextTExtractor = NextTRelationExtractor(programNode);
    auto actual =
        static_pointer_cast<QueryTimeResult>(nextTExtractor.extract(args))
            ->getResult();
    vector<string> expected = {"1", "2", "3"};
    REQUIRE(
        TestDE::DEUtils::vectorStringContainsSameElements(*actual, expected));
  }

  SECTION("Test Valid End Only Arguments With Last Stmt No Of Procedure") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(3);
    StmtNoArgs args = StmtNoArgs();
    args.setEndStmtNo(9);
    NextTRelationExtractor nextTExtractor = NextTRelationExtractor(programNode);
    auto actual =
        static_pointer_cast<QueryTimeResult>(nextTExtractor.extract(args))
            ->getResult();
    vector<string> expected = {"1", "2", "3", "4", "5", "6", "7", "8"};
    REQUIRE(
        TestDE::DEUtils::vectorStringContainsSameElements(*actual, expected));
  }
}
