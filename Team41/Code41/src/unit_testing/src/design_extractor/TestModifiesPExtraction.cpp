//
// Created by Aaron on 23/9/22.
//
#include <unordered_map>

#include "DEUtils.h"
#include "Dummies.h"
#include "catch.hpp"
#include "design_extractor/DesignExtractor.h"
#include "design_extractor/ModifiesExtractor.h"
#include "pkb/DataModifier.h"
#include "pkb/PKBStorage.h"

using namespace std;
using namespace DE;

TEST_CASE("Test ModifiesP Extraction") {
  SECTION("test non-nested + singly-nested procedures") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(6);
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    DesignExtractor designExtractor =
        DesignExtractor(dataModifier, programNode);
    auto actual = ModifiesExtractor::extractModifiesP(programNode);
    vector<vector<string>> expected = {
        {"procedure1", "x"}, {"procedure1", "y"}, {"procedure2", "x"},
        {"procedure2", "y"}, {"procedure2", "w"}, {"procedure2", "foo"}};
    std::list<vector<string>>::iterator it;
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("test singly-nested + doubly-nested procedures") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(7);
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    DesignExtractor designExtractor =
        DesignExtractor(dataModifier, programNode);
    auto actual = ModifiesExtractor::extractModifiesP(programNode);
    vector<vector<string>> expected = {
        {"procedure2", "y"},   {"procedure2", "x"},  {"procedure2", "w"},
        {"procedure2", "foo"}, {"procedure3", "y"},  {"procedure3", "baz"},
        {"procedure3", "w"},   {"procedure3", "foo"}};
    std::list<vector<string>>::iterator it;
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("test doubly-nested + doubly-nested procedures") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(8);
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    DesignExtractor designExtractor =
        DesignExtractor(dataModifier, programNode);
    auto actual = ModifiesExtractor::extractModifiesP(programNode);
    vector<vector<string>> expected = {
        {"procedure3", "y"},   {"procedure3", "baz"}, {"procedure3", "w"},
        {"procedure3", "foo"}, {"procedure5", "x"},   {"procedure5", "y"}};
    std::list<vector<string>>::iterator it;
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("test singly-nested + doubly-nested + doubly-nested procedures") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(9);
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    DesignExtractor designExtractor =
        DesignExtractor(dataModifier, programNode);
    auto actual = ModifiesExtractor::extractModifiesP(programNode);
    vector<vector<string>> expected = {
        {"procedure1", "y"},   {"procedure1", "x"},   {"procedure2", "w"},
        {"procedure2", "x"},   {"procedure2", "foo"}, {"procedure2", "y"},
        {"procedure3", "baz"}, {"procedure3", "w"},   {"procedure3", "y"},
        {"procedure3", "foo"}};
    std::list<vector<string>>::iterator it;
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("test non-nested procedure with single call") {
    auto programNode = TestDE::Dummies::getTestProgramNode(10);
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    DesignExtractor designExtractor =
        DesignExtractor(dataModifier, programNode);
    auto actual = ModifiesExtractor::extractModifiesP(programNode);
    vector<vector<string>> expected = {
        {"procedure2", "x"}, {"procedure2", "z"}, {"procedure3", "z"}};
    std::list<vector<string>>::iterator it;
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("test non-nested procedure with multiple calls") {
    auto programNode = TestDE::Dummies::getTestProgramNode(11);
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    DesignExtractor designExtractor =
        DesignExtractor(dataModifier, programNode);
    auto actual = ModifiesExtractor::extractModifiesP(programNode);
    vector<vector<string>> expected = {
        {"procedure2", "x"},    {"procedure2", "adda"}, {"procedure2", "c"},
        {"procedure3", "adda"}, {"procedure3", "c"},    {"procedure4", "c"}};
    std::list<vector<string>>::iterator it;
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("test nested procedures with multiple calls") {
    auto programNode = TestDE::Dummies::getTestProgramNode(12);
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    DesignExtractor designExtractor =
        DesignExtractor(dataModifier, programNode);
    auto actual = ModifiesExtractor::extractModifiesP(programNode);
    vector<vector<string>> expected = {{"procedure2", "x"},
                                       {"procedure2", "abc"},
                                       {"procedure3", "abc"},
                                       {"procedure6", "abc"}};
    std::list<vector<string>>::iterator it;
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("test doubly-nested procedures with multiple calls") {
    auto programNode = TestDE::Dummies::getTestProgramNode(13);
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    DesignExtractor designExtractor =
        DesignExtractor(dataModifier, programNode);
    auto actual = ModifiesExtractor::extractModifiesP(programNode);
    vector<vector<string>> expected = {
        {"procedure2", "x"},      {"procedure2", "oloha"},
        {"procedure2", "blabla"}, {"procedure2", "zxcv"},
        {"procedure3", "oloha"},  {"procedure4", "blabla"},
        {"procedure4", "zxcv"},   {"procedure6", "zxcv"}};
    std::list<vector<string>>::iterator it;
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }
}