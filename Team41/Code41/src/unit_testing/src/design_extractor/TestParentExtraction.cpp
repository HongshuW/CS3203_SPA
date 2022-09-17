//
// Created by hongshu wang on 10/9/22.
//

#include "catch.hpp"
#include "design_extractor/DesignExtractor.h"
#include "DEUtils.h"
#include "Dummies.h"
#include <unordered_map>
#include "pkb/DataModifier.h"

using namespace std;
using namespace DE;
using namespace TestDE;

TEST_CASE("Test Parent and Parent* Extraction") {
    SECTION("procedure 1") {
        // procedure with no statements
        auto programNode = TestDE::Dummies::getTestProgramNode(0);
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);

        // Test Parent
        shared_ptr<list<vector<string>>> actualParentRelations = designExtractor.extractRelations(RelationType::PARENT);
        vector<vector<string>> expectedParentRelations;
        REQUIRE(expectedParentRelations.size() == actualParentRelations->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actualParentRelations, expectedParentRelations));

        // Test Parent*
        shared_ptr<list<vector<string>>> actualParentTRelations = designExtractor.extractRelations(RelationType::PARENT_T);
        vector<vector<string>> expectedParentTRelations;
        REQUIRE(expectedParentTRelations.size() == actualParentTRelations->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actualParentTRelations, expectedParentTRelations));
    }

    SECTION("procedure 2") {
        // procedure with no parent(*) relations
        auto programNode = TestDE::Dummies::getTestProgramNode(1);
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);

        // Test Parent
        shared_ptr<list<vector<string>>> actualParentRelations = designExtractor.extractRelations(RelationType::PARENT);
        vector<vector<string>> expectedParentRelations;
        REQUIRE(expectedParentRelations.size() == actualParentRelations->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actualParentRelations, expectedParentRelations));

        // Test Parent*
        shared_ptr<list<vector<string>>> actualParentTRelations = designExtractor.extractRelations(RelationType::PARENT_T);
        vector<vector<string>> expectedParentTRelations;
        REQUIRE(expectedParentTRelations.size() == actualParentTRelations->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actualParentTRelations, expectedParentTRelations));
    }

    SECTION("procedure 3") {
        // procedure with 2 nesting levels
        auto programNode = TestDE::Dummies::getTestProgramNode(2);
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);

        // Test Parent
        shared_ptr<list<vector<string>>> actualParentRelations = designExtractor.extractRelations(RelationType::PARENT);
        vector<string> expectedR1 = {"3", "4"};
        vector<string> expectedR2 = {"3", "5"};
        vector<string> expectedR3 = {"3", "6"};
        vector<vector<string>> expectedParentRelations{expectedR1, expectedR2, expectedR3};
        REQUIRE(expectedParentRelations.size() == actualParentRelations->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actualParentRelations, expectedParentRelations));

        // Test Parent*
        shared_ptr<list<vector<string>>> actualParentTRelations = designExtractor.extractRelations(RelationType::PARENT_T);
        // Parent and Parent* should return the same results, since there are only 2 nesting levels
        vector<vector<string>> expectedParentTRelations = expectedParentRelations;
        REQUIRE(expectedParentTRelations.size() == actualParentTRelations->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actualParentTRelations, expectedParentTRelations));
    }

    SECTION("procedure 5") {
        // procedure with 3 nesting levels
        auto programNode = TestDE::Dummies::getTestProgramNode(4);
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);

        // Test Parent
        shared_ptr<list<vector<string>>> actualParentRelations = designExtractor.extractRelations(RelationType::PARENT);
        vector<string> expectedR1 = {"2", "3"};
        vector<string> expectedR2 = {"2", "4"};
        vector<string> expectedR3 = {"4", "5"};
        vector<string> expectedR4 = {"4", "6"};
        vector<vector<string>> expectedParentRelations{expectedR1, expectedR2, expectedR3, expectedR4};
        REQUIRE(expectedParentRelations.size() == actualParentRelations->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actualParentRelations, expectedParentRelations));

        // Test Parent*
        shared_ptr<list<vector<string>>> actualParentTRelations = designExtractor.extractRelations(RelationType::PARENT_T);
        vector<string> expectedR5 = {"2", "5"};
        vector<string> expectedR6 = {"2", "6"};
        vector<vector<string>> expectedParentTRelations{expectedR1, expectedR2, expectedR5, expectedR6, expectedR3, expectedR4};
        REQUIRE(expectedParentTRelations.size() == actualParentTRelations->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actualParentTRelations, expectedParentTRelations));
    }

    SECTION("procedure 6") {
        // procedure with 3 nesting levels and more than 1 while loops in the same nesting level
        auto programNode = TestDE::Dummies::getTestProgramNode(5);
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);

        // Test Parent
        shared_ptr<list<vector<string>>> actualParentRelations = designExtractor.extractRelations(RelationType::PARENT);
        vector<string> expectedR1 = {"2", "3"};
        vector<string> expectedR2 = {"2", "4"};
        vector<string> expectedR3 = {"4", "5"};
        vector<string> expectedR4 = {"4", "6"};
        vector<string> expectedR5 = {"8", "9"};
        vector<vector<string>> expectedParentRelations{expectedR1, expectedR2, expectedR3, expectedR4, expectedR5};
        REQUIRE(expectedParentRelations.size() == actualParentRelations->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actualParentRelations, expectedParentRelations));

        // Test Parent*
        shared_ptr<list<vector<string>>> actualParentTRelations = designExtractor.extractRelations(RelationType::PARENT_T);
        vector<string> expectedR6 = {"2", "5"};
        vector<string> expectedR7 = {"2", "6"};
        vector<vector<string>> expectedParentTRelations{expectedR1, expectedR2, expectedR3, expectedR4, expectedR5, expectedR6, expectedR7};
        REQUIRE(expectedParentTRelations.size() == actualParentTRelations->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actualParentTRelations, expectedParentTRelations));
    }
}