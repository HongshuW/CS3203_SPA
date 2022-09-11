//
// Created by Nafour on 11/9/22.
//
#include "catch.hpp"
#include <unordered_map>
#include "design_extractor/DesignExtractor.h"
#include "pkb/DataModifier.h"
#include "Dummies.h"
#include "DEUtils.h"
using namespace std;
using namespace DE;

TEST_CASE("Test Uses_S Extraction") {
    SECTION("test empty procedure") {
        auto programNode = TestDE::Dummies::getTestProgramNode(0);
        DataModifier dataModifier = DataModifier();
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        auto actual = designExtractor.extractRelations(QB::RelationType::USES_S);
        REQUIRE(actual->empty());

    }
    SECTION("test non-nested procedure") {
        auto programNode = TestDE::Dummies::getTestProgramNode(1);
        DataModifier dataModifier = DataModifier();
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        auto actual = designExtractor.extractRelations(QB::RelationType::USES_S);
        vector<vector<string>> expected = {{"1", "x"}, {"3", "y"}};
        std::list<vector<string>>::iterator it;
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }
    SECTION("test singly-nested procedure") {
        auto programNode = TestDE::Dummies::getTestProgramNode(2);
        DataModifier dataModifier = DataModifier();
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        auto actual = designExtractor.extractRelations(QB::RelationType::USES_S);
        vector<vector<string>> expected = {{"1", "x"}, {"3", "bar"},  {"3", "y"},  {"4", "y"}, {"5", "z"}, {"3", "z"}};
        std::list<vector<string>>::iterator it;
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }
    SECTION("test doubly-nested procedure") {
        auto programNode = TestDE::Dummies::getTestProgramNode(3);
        DataModifier dataModifier = DataModifier();
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        auto actual = designExtractor.extractRelations(QB::RelationType::USES_S);
        vector<vector<string>> expected = {{"1", "x"},
                                           {"3", "bar"},
                                           {"3", "y"},
                                           {"3", "z"},
                                           {"3", "baz"},
                                           {"3", "qux"},
                                           {"3", "quux"},
                                           {"3", "haha"},
                                           {"4", "z"},
                                           {"5", "qux"},
                                           {"5", "quux"},
                                           {"5", "haha"},
                                           {"5", "bar"},
                                           {"5", "baz"},
                                           {"7", "bar"},
        };
        std::list<vector<string>>::iterator it;
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }
}