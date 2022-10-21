//
// Created by Aaron on 23/9/22.
//
#include <unordered_map>

#include "DEUtils.h"
#include "Dummies.h"
#include "catch.hpp"
#include "design_extractor/DesignExtractor.h"
#include "design_extractor/UsesExtractor.h"
#include "pkb/DataModifier.h"
#include "pkb/PKBStorage.h"

using namespace std;
using namespace DE;

TEST_CASE("Test UsesP Extraction") {
  SECTION("test non-nested + singly-nested procedures") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(6);
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    DesignExtractor designExtractor =
        DesignExtractor(dataModifier, programNode);
    auto actual = UsesExtractor::extractUsesP(programNode);
    vector<vector<string>> expected = {
        {"procedure1", "x"},   {"procedure1", "y"}, {"procedure2", "x"},
        {"procedure2", "bar"}, {"procedure2", "y"}, {"procedure2", "z"}};
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
    auto actual = UsesExtractor::extractUsesP(programNode);
    vector<vector<string>> expected = {
        {"procedure2", "x"},    {"procedure2", "bar"},  {"procedure2", "z"},
        {"procedure2", "y"},    {"procedure3", "x"},    {"procedure3", "qux"},
        {"procedure3", "haha"}, {"procedure3", "quux"}, {"procedure3", "baz"},
        {"procedure3", "y"},    {"procedure3", "z"},    {"procedure3", "bar"}};
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
    auto actual = UsesExtractor::extractUsesP(programNode);
    vector<vector<string>> expected = {
        {"procedure3", "x"},    {"procedure3", "bar"},  {"procedure3", "qux"},
        {"procedure3", "quux"}, {"procedure3", "baz"},  {"procedure3", "y"},
        {"procedure3", "z"},    {"procedure3", "haha"}, {"procedure5", "x"}};
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
    auto actual = UsesExtractor::extractUsesP(programNode);
    vector<vector<string>> expected = {
        {"procedure1", "x"},    {"procedure1", "y"},    {"procedure2", "x"},
        {"procedure2", "bar"},  {"procedure2", "y"},    {"procedure2", "z"},
        {"procedure3", "x"},    {"procedure3", "bar"},  {"procedure3", "qux"},
        {"procedure3", "haha"}, {"procedure3", "quux"}, {"procedure3", "baz"},
        {"procedure3", "y"},    {"procedure3", "z"}};
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
    auto actual = UsesExtractor::extractUsesP(programNode);
    vector<vector<string>> expected = {{"procedure2", "x"},
                                       {"procedure2", "dah"},
                                       {"procedure2", "y"},
                                       {"procedure3", "dah"}};
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
    auto actual = UsesExtractor::extractUsesP(programNode);
    vector<vector<string>> expected = {
        {"procedure2", "x"}, {"procedure2", "y"}, {"procedure2", "a"},
        {"procedure2", "b"}, {"procedure2", "z"}, {"procedure3", "y"},
        {"procedure3", "z"}, {"procedure3", "a"}, {"procedure3", "b"},
        {"procedure4", "z"}, {"procedure4", "a"}, {"procedure4", "b"}};
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
    auto actual = UsesExtractor::extractUsesP(programNode);
    vector<vector<string>> expected = {
        {"procedure2", "x"},   {"procedure2", "y"},      {"procedure2", "def"},
        {"procedure2", "xyz"}, {"procedure2", "qwerty"}, {"procedure2", "z"},
        {"procedure3", "y"},   {"procedure3", "z"},      {"procedure3", "def"},
        {"procedure3", "xyz"}, {"procedure4", "z"},      {"procedure6", "xyz"},
        {"procedure6", "def"}};
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
    auto actual = UsesExtractor::extractUsesP(programNode);
    vector<vector<string>> expected = {
        {"procedure2", "x"},   {"procedure2", "a"},    {"procedure2", "y"},
        {"procedure2", "b"},   {"procedure2", "asdf"}, {"procedure2", "z"},
        {"procedure2", "xxx"}, {"procedure2", "vvv"},  {"procedure3", "y"},
        {"procedure3", "b"},   {"procedure3", "asdf"}, {"procedure4", "z"},
        {"procedure4", "xxx"}, {"procedure4", "vvv"},  {"procedure5", "asdf"},
        {"procedure6", "vvv"}, {"procedure6", "xxx"}};
    std::list<vector<string>>::iterator it;
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }
}