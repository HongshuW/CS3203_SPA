//
// Created by Nafour on 11/9/22.
//
#include <unordered_map>

#include "DEUtils.h"
#include "Dummies.h"
#include "catch.hpp"
#include "design_extractor/DesignExtractor.h"
#include "design_extractor/UsesExtractor.h"
#include "design_extractor/extractors/UsesSRelationExtractor.h"
#include "design_extractor/results/RelationResult.h"
#include "pkb/DataModifier.h"

using namespace std;
using namespace DE;

TEST_CASE("Test Uses_S Extraction") {
  SECTION("test empty procedure") {
    auto programNode = TestDE::Dummies::getTestProgramNode(0);
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    auto actual =
        static_pointer_cast<RelationResult>(
            UsesSRelationExtractor(dataModifier, programNode).extract())
            ->getResult();
    REQUIRE(actual->empty());
  }
  SECTION("test non-nested procedure") {
    auto programNode = TestDE::Dummies::getTestProgramNode(1);
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    auto actual =
        static_pointer_cast<RelationResult>(
            UsesSRelationExtractor(dataModifier, programNode).extract())
            ->getResult();
    vector<vector<string>> expected = {{"1", "x"}, {"3", "y"}};
    std::list<vector<string>>::iterator it;
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }
  SECTION("test singly-nested procedure") {
    auto programNode = TestDE::Dummies::getTestProgramNode(2);
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    auto actual =
        static_pointer_cast<RelationResult>(
            UsesSRelationExtractor(dataModifier, programNode).extract())
            ->getResult();
    vector<vector<string>> expected = {{"1", "x"}, {"3", "bar"}, {"3", "y"},
                                       {"4", "y"}, {"5", "z"},   {"3", "z"}};
    std::list<vector<string>>::iterator it;
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }
  SECTION("test doubly-nested procedure") {
    auto programNode = TestDE::Dummies::getTestProgramNode(3);
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    auto actual =
        static_pointer_cast<RelationResult>(
            UsesSRelationExtractor(dataModifier, programNode).extract())
            ->getResult();
    vector<vector<string>> expected = {
        {"1", "x"},    {"3", "bar"},  {"3", "y"},    {"3", "z"},   {"3", "baz"},
        {"3", "qux"},  {"3", "quux"}, {"3", "haha"}, {"4", "z"},   {"5", "qux"},
        {"5", "quux"}, {"5", "haha"}, {"5", "bar"},  {"5", "baz"}, {"7", "bar"},
    };
    std::list<vector<string>>::iterator it;
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("test non-nested procedure with single call") {
    auto programNode = TestDE::Dummies::getTestProgramNode(10);
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    auto actual =
        static_pointer_cast<RelationResult>(
            UsesSRelationExtractor(dataModifier, programNode).extract())
            ->getResult();
    vector<vector<string>> expected = {
        {"1", "x"}, {"2", "dah"}, {"3", "y"}, {"4", "dah"}};
    std::list<vector<string>>::iterator it;
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("test non-nested procedure with multiple calls") {
    auto programNode = TestDE::Dummies::getTestProgramNode(11);
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    auto actual =
        static_pointer_cast<RelationResult>(
            UsesSRelationExtractor(dataModifier, programNode).extract())
            ->getResult();
    vector<vector<string>> expected = {
        {"1", "x"}, {"2", "y"}, {"2", "z"}, {"2", "a"}, {"2", "b"}, {"3", "z"},
        {"3", "a"}, {"3", "b"}, {"4", "y"}, {"5", "y"}, {"7", "z"}, {"7", "a"},
        {"7", "b"}, {"8", "z"}, {"9", "a"}, {"9", "b"}};
    std::list<vector<string>>::iterator it;
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("test nested procedures with multiple calls") {
    auto programNode = TestDE::Dummies::getTestProgramNode(12);
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    auto actual =
        static_pointer_cast<RelationResult>(
            UsesSRelationExtractor(dataModifier, programNode).extract())
            ->getResult();
    vector<vector<string>> expected = {
        {"1", "x"},   {"2", "x"},   {"2", "qwerty"}, {"2", "y"},
        {"2", "z"},   {"2", "xyz"}, {"2", "def"},    {"3", "y"},
        {"3", "z"},   {"3", "def"}, {"3", "xyz"},    {"4", "qwerty"},
        {"5", "z"},   {"6", "y"},   {"7", "y"},      {"8", "z"},
        {"9", "def"}, {"9", "xyz"}, {"10", "z"},     {"11", "xyz"},
        {"11", "def"}};
    std::list<vector<string>>::iterator it;
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("test doubly-nested procedures with multiple calls") {
    auto programNode = TestDE::Dummies::getTestProgramNode(13);
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    auto actual =
        static_pointer_cast<RelationResult>(
            UsesSRelationExtractor(dataModifier, programNode).extract())
            ->getResult();
    vector<vector<string>> expected = {
        {"1", "x"},    {"2", "a"},    {"2", "x"},     {"2", "y"},
        {"2", "b"},    {"2", "asdf"}, {"2", "z"},     {"2", "vvv"},
        {"2", "xxx"},  {"3", "x"},    {"3", "y"},     {"3", "b"},
        {"3", "z"},    {"3", "asdf"}, {"3", "xxx"},   {"3", "vvv"},
        {"4", "y"},    {"4", "b"},    {"4", "asdf"},  {"5", "z"},
        {"5", "xxx"},  {"5", "vvv"},  {"6", "y"},     {"7", "y"},
        {"8", "b"},    {"8", "asdf"}, {"9", "asdf"},  {"11", "z"},
        {"12", "xxx"}, {"12", "vvv"}, {"14", "asdf"}, {"15", "vvv"},
        {"15", "xxx"}};
    std::list<vector<string>>::iterator it;
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }
}