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

TEST_CASE("Test Parent and Parent* Extraction") {
    SECTION("procedure 5") {
        auto programNode = TestDE::Dummies::getTestProgramNode(4);
        DataModifier dataModifier = DataModifier();
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
}