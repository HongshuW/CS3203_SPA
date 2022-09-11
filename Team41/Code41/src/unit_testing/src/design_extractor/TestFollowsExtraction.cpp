//
// Created by Aaron on 10/9/22.
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

TEST_CASE("Test Follows Extractor") {
    SECTION("test empty procedure") {
        auto programNode = TestDE::Dummies::getTestProgramNode(0);
        DataModifier dataModifier = DataModifier();
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        auto actual = designExtractor.extractRelations(QB::RelationType::FOLLOWS);
        REQUIRE(actual->empty());
    }

    SECTION("test non-nested procedure") {
        auto programNode = TestDE::Dummies::getTestProgramNode(1);
        DataModifier dataModifier = DataModifier();
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        auto actual = designExtractor.extractRelations(QB::RelationType::FOLLOWS);
        vector<vector<string>> expected = {{"1", "2"}, {"2", "3"}};
        std::list<vector<string>>::iterator it;
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }

    SECTION("test singly-nested procedure") {
        auto programNode = TestDE::Dummies::getTestProgramNode(2);
        DataModifier dataModifier = DataModifier();
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        auto actual = designExtractor.extractRelations(QB::RelationType::FOLLOWS);
        vector<vector<string>> expected = { {"1", "2"}, {"2", "3"}, {"3", "7"}, {"4", "5"}};
        std::list<vector<string>>::iterator it;
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }

    SECTION("test doubly-nested procedure") {
        auto programNode = TestDE::Dummies::getTestProgramNode(3);
        DataModifier dataModifier = DataModifier();
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        auto actual = designExtractor.extractRelations(QB::RelationType::FOLLOWS);
        vector<vector<string>> expected = { {"1", "2"}, {"2", "3"}, {"3", "9"}, {"4", "5"}, {"6", "7"}};
        std::list<vector<string>>::iterator it;
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }
}