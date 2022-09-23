//
// Created by Aaron on 23/9/22.
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

TEST_CASE("Test UsesP Extraction") {
    SECTION("test non-nested + singly-nested procedures") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(6);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        auto actual = designExtractor.extractRelations(QB::RelationType::USES_P);
        vector<vector<string>> expected = { {"procedure1", "x"}, {"procedure1", "y"}, {"procedure2", "x"},
                                            {"procedure2", "bar"}, {"procedure2", "y"}, {"procedure2", "z"},
                                            {"procedure2", "y"}, {"procedure2", "z"},};
        std::list<vector<string>>::iterator it;
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }

    SECTION("test singly-nested + doubly-nested procedures") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(7);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        auto actual = designExtractor.extractRelations(QB::RelationType::USES_P);
        vector<vector<string>> expected = { {"procedure2", "x"}, {"procedure2", "bar"}, {"procedure2", "z"},
                                            {"procedure2", "y"}, {"procedure2", "y"},
                                            {"procedure2", "z"}, {"procedure3", "x"}, {"procedure3", "bar"},
                                            {"procedure3", "qux"}, {"procedure3", "haha"}, {"procedure3", "quux"},
                                            {"procedure3", "baz"},
                                            {"procedure3", "y"}, {"procedure3", "z"}, {"procedure3", "z"},
                                            {"procedure3", "baz"}, {"procedure3", "quux"},
                                            {"procedure3", "haha"}, {"procedure3", "qux"}, {"procedure3", "bar"},
                                            {"procedure3", "bar"}};
        std::list<vector<string>>::iterator it;
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }

    SECTION("test doubly-nested + doubly-nested procedures") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(8);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        auto actual = designExtractor.extractRelations(QB::RelationType::USES_P);
        vector<vector<string>> expected = { {"procedure3", "x"}, {"procedure3", "bar"},
                                            {"procedure3", "qux"}, {"procedure3", "haha"},
                                            {"procedure3", "quux"},
                                            {"procedure3", "baz"}, {"procedure3", "y"}, {"procedure3", "z"},
                                            {"procedure3", "z"}, {"procedure3", "baz"},
                                            {"procedure3", "quux"}, {"procedure3", "haha"},
                                            {"procedure3", "qux"}, {"procedure3", "bar"}, {"procedure3", "bar"},
                                            {"procedure5", "x"}, {"procedure5", "x"}};
        std::list<vector<string>>::iterator it;
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }

    SECTION("test singly-nested + doubly-nested + doubly-nested procedures") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(9);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        auto actual = designExtractor.extractRelations(QB::RelationType::USES_P);
        vector<vector<string>> expected = {{"procedure1",  "x"}, {"procedure1",  "y"},
                                           {"procedure2",  "x"}, {"procedure2",  "bar"}, {"procedure2",  "y"},
                                           {"procedure2",  "z"}, {"procedure2", "y"}, {"procedure2", "z"},
                                           {"procedure3", "x"}, {"procedure3", "bar"}, {"procedure3", "qux"},
                                           {"procedure3", "haha"}, {"procedure3", "quux"}, {"procedure3", "baz"},
                                           {"procedure3", "y"}, {"procedure3", "z"}, {"procedure3", "z"},
                                           {"procedure3", "baz"}, {"procedure3", "quux"}, {"procedure3", "haha"},
                                           {"procedure3", "qux"},
                                           {"procedure3", "bar"}, {"procedure3", "bar"}};
        std::list<vector<string>>::iterator it;
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }
}