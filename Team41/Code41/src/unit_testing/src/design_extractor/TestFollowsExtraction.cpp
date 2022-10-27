//
// Created by Aaron on 10/9/22.
//
#include <unordered_map>

#include "AST/IfNode.h"
#include "AST/PrintNode.h"
#include "AST/ReadNode.h"
#include "AST/WhileNode.h"
#include "AST/utils/ASTUtils.h"
#include "DEUtils.h"
#include "Dummies.h"
#include "catch.hpp"
#include "design_extractor/DesignExtractor.h"
#include "design_extractor/FollowsExtractor.h"
#include "iostream"
#include "pkb/DataModifier.h"

using namespace std;
using namespace DE;
#include "design_extractor/extractors/FollowsRelationExtractor.h"

TEST_CASE("Test Follows Extractor") {
  SECTION("test empty procedure") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(0);
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    auto actual =
        static_pointer_cast<RelationResult>(
            FollowsRelationExtractor(dataModifier, programNode).extract())
            ->getResult();
    REQUIRE(actual->empty());
  }

  SECTION("test non-nested procedure") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(1);
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);

    auto actual =
        static_pointer_cast<RelationResult>(
            FollowsRelationExtractor(dataModifier, programNode).extract())
            ->getResult();
    vector<vector<string>> expected = {{"1", "2"}, {"2", "3"}};
    std::list<vector<string>>::iterator it;
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("test singly-nested procedure") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(2);
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    DesignExtractor designExtractor =
        DesignExtractor(dataModifier, programNode);
    auto actual =
        static_pointer_cast<RelationResult>(
            FollowsRelationExtractor(dataModifier, programNode).extract())
            ->getResult();
    vector<vector<string>> expected = {
        {"1", "2"}, {"2", "3"}, {"3", "7"}, {"4", "5"}};
    std::list<vector<string>>::iterator it;
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("test doubly-nested procedure") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(3);
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    DesignExtractor designExtractor =
        DesignExtractor(dataModifier, programNode);
    auto actual =
        static_pointer_cast<RelationResult>(
            FollowsRelationExtractor(dataModifier, programNode).extract())
            ->getResult();
    vector<vector<string>> expected = {
        {"1", "2"}, {"2", "3"}, {"3", "9"}, {"4", "5"}, {"6", "7"}};
    std::list<vector<string>>::iterator it;
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("test non-nested + singly-nested procedures") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(6);
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    DesignExtractor designExtractor =
        DesignExtractor(dataModifier, programNode);
    auto actual =
        static_pointer_cast<RelationResult>(
            FollowsRelationExtractor(dataModifier, programNode).extract())
            ->getResult();
    vector<vector<string>> expected = {{"1", "2"}, {"2", "3"},  {"4", "5"},
                                       {"5", "6"}, {"6", "10"}, {"7", "8"}};
    std::list<vector<string>>::iterator it;
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("test singly-nested + doubly-nested procedures") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(7);
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    auto actual =
        static_pointer_cast<RelationResult>(
            FollowsRelationExtractor(dataModifier, programNode).extract())
            ->getResult();
    vector<vector<string>> expected = {
        {"1", "2"},  {"2", "3"},   {"3", "7"},   {"4", "5"},  {"8", "9"},
        {"9", "10"}, {"10", "16"}, {"11", "12"}, {"13", "14"}};
    std::list<vector<string>>::iterator it;
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("test doubly-nested + doubly-nested procedures") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(8);
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    auto actual =
        static_pointer_cast<RelationResult>(
            FollowsRelationExtractor(dataModifier, programNode).extract())
            ->getResult();
    vector<vector<string>> expected = {{"1", "2"},   {"2", "3"},  {"3", "9"},
                                       {"4", "5"},   {"6", "7"},  {"10", "11"},
                                       {"11", "16"}, {"12", "13"}};
    std::list<vector<string>>::iterator it;
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("test singly-nested + doubly-nested + doubly-nested procedures") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(9);
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    auto actual =
        static_pointer_cast<RelationResult>(
            FollowsRelationExtractor(dataModifier, programNode).extract())
            ->getResult();
    vector<vector<string>> expected = {{"1", "2"},   {"2", "3"},   {"4", "5"},
                                       {"5", "6"},   {"6", "10"},  {"7", "8"},
                                       {"11", "12"}, {"12", "13"}, {"13", "19"},
                                       {"14", "15"}, {"16", "17"}};
    std::list<vector<string>>::iterator it;
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }
}
