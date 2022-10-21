//
// Created by Aaron on 23/9/22.
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
#include "design_extractor/CallsExtractor.h"
#include "design_extractor/DesignExtractor.h"
#include "iostream"
#include "pkb/DataModifier.h"

using namespace std;
using namespace DE;

TEST_CASE("Test Call Extractor") {
  SECTION("Test Empty Procedure") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(0);
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    DesignExtractor designExtractor =
        DesignExtractor(dataModifier, programNode);
    auto actual = CallsExtractor::extractCalls(programNode);
    REQUIRE(actual->empty());
  }
  SECTION("Test Call One Procedure") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(10);
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    DesignExtractor designExtractor =
        DesignExtractor(dataModifier, programNode);
    auto actual = CallsExtractor::extractCalls(programNode);
    vector<vector<string>> expected = {{"procedure2", "procedure3", "2"}};
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementTuple(*actual, expected));
  }
  SECTION("Test Two Procedures Call The Same Procedures") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(11);
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    DesignExtractor designExtractor =
        DesignExtractor(dataModifier, programNode);
    auto actual = CallsExtractor::extractCalls(programNode);
    vector<vector<string>> expected = {{"procedure2", "procedure3", "2"},
                                       {"procedure2", "procedure4", "3"},
                                       {"procedure3", "procedure4", "7"}};
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementTuple(*actual, expected));
  }

  SECTION("Test Call In Singly-Nested Procedure") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(12);
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    DesignExtractor designExtractor =
        DesignExtractor(dataModifier, programNode);
    auto actual = CallsExtractor::extractCalls(programNode);
    vector<vector<string>> expected = {{"procedure2", "procedure3", "3"},
                                       {"procedure2", "procedure4", "5"},
                                       {"procedure3", "procedure4", "8"},
                                       {"procedure3", "procedure6", "9"}};
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementTuple(*actual, expected));
  }

  SECTION("Test Call In Doubly-Nested Procedure") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(13);
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    DesignExtractor designExtractor =
        DesignExtractor(dataModifier, programNode);
    auto actual = CallsExtractor::extractCalls(programNode);
    vector<vector<string>> expected = {{"procedure2", "procedure3", "4"},
                                       {"procedure2", "procedure4", "5"},
                                       {"procedure3", "procedure5", "9"},
                                       {"procedure4", "procedure6", "12"}};
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementTuple(*actual, expected));
  }
}
