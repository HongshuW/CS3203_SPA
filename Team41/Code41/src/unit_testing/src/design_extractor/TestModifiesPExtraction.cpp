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

TEST_CASE("Test ModifiesP Extraction") {
    SECTION("test non-nested + singly-nested procedures") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(6);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        auto actual = designExtractor.extractRelations(QB::RelationType::MODIFIES_P);
        vector<vector<string>> expected = { {"2", "y"}, {"3", "x"}, {"6", "x"}, {"5", "y"}, {"6", "w"},
                                            {"7", "x"}, {"9", "w"}, {"10", "foo"}};
        std::list<vector<string>>::iterator it;
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }

    SECTION("test singly-nested + doubly-nested procedures") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(7);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        auto actual = designExtractor.extractRelations(QB::RelationType::MODIFIES_P);
        vector<vector<string>> expected = { {"2", "y"}, {"3", "w"}, {"3", "x"}, {"4", "x"}, {"6", "w"},
                                            {"7", "foo"}, {"9", "y"}, {"10", "baz"},
                                            {"10", "w"}, {"12", "baz"}, {"13", "baz"}, {"15", "w"},
                                            {"16", "foo"}};
        std::list<vector<string>>::iterator it;
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }

    SECTION("test doubly-nested + doubly-nested procedures") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(8);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        auto actual = designExtractor.extractRelations(QB::RelationType::MODIFIES_P);
        vector<vector<string>> expected = { {"2", "y"}, {"3", "baz"}, {"3", "w"}, {"5", "baz"}, {"6", "baz"},
                                            {"8", "w"}, {"9", "foo"}, {"10", "x"},
                                            {"11", "x"}, {"11", "y"}, {"12", "x"}, {"13", "y"},
                                            {"14", "y"}, {"15", "y"}, {"16", "y"}};
        std::list<vector<string>>::iterator it;
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }

    SECTION("test singly-nested + doubly-nested + doubly-nested procedures") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(9);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        auto actual = designExtractor.extractRelations(QB::RelationType::FOLLOWS_T);
        vector<vector<string>> expected = {{"1",  "2"}, {"1",  "3"},
                                           {"2",  "3"}, {"4",  "5"}, {"4",  "6"},
                                           {"4",  "10"}, {"5", "6"}, {"5", "10"},
                                           {"6", "10"}, {"7", "8"}, {"11", "12"},
                                           {"11", "13"}, {"11", "19"}, {"12", "13"},
                                           {"12", "19"}, {"13", "19"}, {"14", "15"}, {"16", "17"}};
        std::list<vector<string>>::iterator it;
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }

    SECTION("test singly-nested + doubly-nested + doubly-nested procedures") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(9);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        auto actual = designExtractor.extractRelations(QB::RelationType::MODIFIES_P);
        vector<vector<string>> expected = {{"2",  "y"}, {"3",  "x"},
                                           {"5",  "y"}, {"6",  "w"}, {"6",  "x"},
                                           {"7",  "x"}, {"9", "w"}, {"10", "foo"},
                                           {"12", "y"}, {"13", "baz"}, {"13", "w"},
                                           {"15", "baz"}, {"16", "baz"}, {"18", "w"},
                                           {"19", "foo"}};
        std::list<vector<string>>::iterator it;
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }
}