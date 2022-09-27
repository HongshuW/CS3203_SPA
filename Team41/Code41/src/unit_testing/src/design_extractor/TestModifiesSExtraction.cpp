//
// Created by hongshu wang on 11/9/22.
//

#include "catch.hpp"
#include "design_extractor/DesignExtractor.h"
#include "DEUtils.h"
#include "Dummies.h"
#include <unordered_map>
#include "pkb/DataModifier.h"
#include "pkb/PKBStorage.h"

using namespace std;
using namespace DE;

TEST_CASE("Test ModifiesS Extraction") {
    SECTION("procedure 1") {
        // procedure with no statements
        auto programNode = TestDE::Dummies::getTestProgramNode(0);
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);

        shared_ptr<list<vector<string>>> actualModifiesSRelations = designExtractor.extractRelations(RelationType::MODIFIES_S);
        vector<vector<string>> expectedModifiesSRelations;
        REQUIRE(expectedModifiesSRelations.size() == actualModifiesSRelations->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actualModifiesSRelations, expectedModifiesSRelations));
    }

    SECTION("procedure 2") {
        // procedure with no nesting
        auto programNode = TestDE::Dummies::getTestProgramNode(1);
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);

        shared_ptr<list<vector<string>>> actualModifiesSRelations = designExtractor.extractRelations(RelationType::MODIFIES_S);
        vector<string> expectedModifiesSR1{"2", "y"};
        vector<string> expectedModifiesSR2{"3", "x"};
        vector<vector<string>> expectedModifiesSRelations{expectedModifiesSR1, expectedModifiesSR2};
        REQUIRE(expectedModifiesSRelations.size() == actualModifiesSRelations->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actualModifiesSRelations, expectedModifiesSRelations));
    }

    SECTION("procedure 5") {
        // complex procedure
        auto programNode = TestDE::Dummies::getTestProgramNode(4);
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);

        shared_ptr<list<vector<string>>> actualModifiesSRelations = designExtractor.extractRelations(RelationType::MODIFIES_S);
        vector<string> expectedModifiesSR1{"1", "x"};
        vector<string> expectedModifiesSR2{"2", "x"};
        vector<string> expectedModifiesSR3{"2", "y"};
        vector<string> expectedModifiesSR4{"3", "x"};
        vector<string> expectedModifiesSR5{"4", "y"};
        vector<string> expectedModifiesSR6{"5", "y"};
        vector<string> expectedModifiesSR7{"6", "y"};
        vector<string> expectedModifiesSR8{"7", "y"};
        vector<vector<string>> expectedModifiesSRelations{expectedModifiesSR1, expectedModifiesSR2, expectedModifiesSR3,
                                                        expectedModifiesSR4, expectedModifiesSR5, expectedModifiesSR6,
                                                        expectedModifiesSR7, expectedModifiesSR8};
        REQUIRE(expectedModifiesSRelations.size() == actualModifiesSRelations->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actualModifiesSRelations, expectedModifiesSRelations));
    }

    SECTION("test non-nested procedure with single call") {
        auto programNode = TestDE::Dummies::getTestProgramNode(10);
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        auto actual = designExtractor.extractRelations(QB::RelationType::MODIFIES_S);
        vector<vector<string>> expected = {{"2", "z"}, {"3", "x"}, {"5", "z"}};
        std::list<vector<string>>::iterator it;
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }

    SECTION("test non-nested procedure with multiple calls") {
        auto programNode = TestDE::Dummies::getTestProgramNode(11);
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        auto actual = designExtractor.extractRelations(QB::RelationType::MODIFIES_S);
        vector<vector<string>> expected = {{"2", "c"}, {"2", "adda"}, {"3", "c"},
                                           {"4", "x"}, {"6", "adda"}, {"7", "c"},
                                           {"9", "c"}};
        std::list<vector<string>>::iterator it;
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }

    SECTION("test nested procedures with multiple calls") {
        auto programNode = TestDE::Dummies::getTestProgramNode(12);
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        auto actual = designExtractor.extractRelations(QB::RelationType::MODIFIES_S);
        vector<vector<string>> expected = {{"3", "abc"}, {"6", "x"}, {"9", "abc"},
                                           {"11", "abc"}};
        std::list<vector<string>>::iterator it;
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }

    SECTION("test doubly-nested procedures with multiple calls") {
        auto programNode = TestDE::Dummies::getTestProgramNode(13);
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        auto actual = designExtractor.extractRelations(QB::RelationType::MODIFIES_S);
        vector<vector<string>> expected = {{"4", "oloha"}, {"5", "blabla"}, {"5", "zxcv"},
                                           {"6", "x"}, {"8", "oloha"}, {"10", "oloha"},
                                           {"12", "zxcv"}, {"13", "blabla"}, {"15", "zxcv"}};
        std::list<vector<string>>::iterator it;
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }
}