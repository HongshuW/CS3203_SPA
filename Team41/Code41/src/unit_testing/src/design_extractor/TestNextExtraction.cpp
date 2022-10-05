//
// Created by Aaron on 2/10/22.
//
#include "catch.hpp"
#include "design_extractor/DesignExtractor.h"
#include "design_extractor/FollowsExtractor.h"
#include "AST/PrintNode.h"
#include "AST/ReadNode.h"
#include "AST/IfNode.h"
#include "AST/WhileNode.h"
#include "AST/utils/ASTUtils.h"
#include <unordered_map>
#include "pkb/DataModifier.h"
#include "iostream"
#include "Dummies.h"
#include "DEUtils.h"

using namespace std;
using namespace DE;

TEST_CASE("Test Next Extractor") {
    SECTION("Test Empty Procedure") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(0);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        auto actual = designExtractor.extractRelations(QB::RelationType::NEXT);
        vector<vector<string>> expected = {};
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }

    SECTION("Test Procedure With Assign Only") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(1);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        auto actual = designExtractor.extractRelations(QB::RelationType::NEXT);
        vector<vector<string>> expected = { {"1", "2"}, {"2", "3"} };
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }

    SECTION("Test Procedure With If Statments") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(2);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        auto actual = designExtractor.extractRelations(QB::RelationType::NEXT);
        vector<vector<string>> expected = { {"1", "2"}, {"2", "3"}, {"3", "4"}, 
                                            {"3", "6"}, {"4", "5"}, {"5", "7"}, {"6", "7"}};
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }

    SECTION("Test Procedure With Nested If Statments In While Loop") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(4);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        auto actual = designExtractor.extractRelations(QB::RelationType::NEXT);
        vector<vector<string>> expected = { {"1", "2"}, {"2", "3"}, {"2", "7"}, {"3", "4"},
                                            {"4", "5"}, {"4", "6"}, {"5", "2"}, {"6", "2"}};
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }
}