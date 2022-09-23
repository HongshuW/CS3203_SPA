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
        vector<vector<string>> expected = { {"1", "x"}, {"3", "y"}, {"4", "x"},
                                            {"6", "bar"}, {"6", "y"}, {"6", "z"},
                                            {"7", "y"}, {"8", "z"},};
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
        vector<vector<string>> expected = { {"1", "x"}, {"3", "bar"}, {"3", "z"}, {"3", "y"}, {"4", "y"},
                                            {"5", "z"}, {"8", "x"}, {"10", "bar"},
                                            {"10", "qux"}, {"10", "haha"}, {"10", "quux"}, {"10", "baz"},
                                            {"10", "y"}, {"10", "z"}, {"11", "z"}, {"12", "baz"},
                                            {"12", "quux"}, {"12", "haha"}, {"12", "qux"}, {"12", "bar"},
                                            {"14", "bar"}};
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
        vector<vector<string>> expected = { {"1", "x"}, {"3", "bar"}, {"3", "qux"}, {"3", "haha"}, {"3", "quux"},
                                            {"3", "baz"}, {"3", "y"}, {"3", "z"},
                                            {"4", "z"}, {"5", "baz"}, {"5", "quux"}, {"5", "haha"},
                                            {"5", "qux"}, {"5", "bar"}, {"7", "bar"},
                                            {"11", "x"}, {"13", "x"}};
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
        vector<vector<string>> expected = {{"1",  "x"}, {"3",  "y"},
                                           {"4",  "x"}, {"6",  "bar"}, {"6",  "y"},
                                           {"6",  "z"}, {"7", "y"}, {"8", "z"},
                                           {"11", "x"}, {"13", "bar"}, {"13", "qux"},
                                           {"13", "haha"}, {"13", "quux"}, {"13", "baz"},
                                           {"13", "y"}, {"13", "z"}, {"14", "z"},
                                           {"15", "baz"}, {"15", "quux"}, {"15", "haha"}, {"15", "qux"},
                                           {"15", "bar"}, {"17", "bar"}};
        std::list<vector<string>>::iterator it;
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }
}