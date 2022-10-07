//
// Created by Aaron on 3/10/22.
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

TEST_CASE("Test Affects Extractor") {
    SECTION("Test No Arguments") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(17);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        StmtNoArgs args = StmtNoArgs();
        list<vector<string>> actual = designExtractor.getAllAffectsRelations();
        vector<vector<string>> expected = { {"1", "5"}, {"3", "5"} };
        REQUIRE(expected.size() == actual.size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(actual, expected));
    }

    SECTION("Test No Arguments With Normal Procedure") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(21);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        StmtNoArgs args = StmtNoArgs();
        list<vector<string>> actual = designExtractor.getAllAffectsRelations();
        vector<vector<string>> expected = {
                {"1", "4"}, {"1", "8"}, {"1", "10"},
                {"1", "12"}, {"10", "11"}, {"10", "12"}, {"11", "12"},
                {"2", "10"}, {"2", "6"},
                {"8", "10"}, {"8", "12"},
                {"4", "4"}, {"4", "10"}, {"4", "12"}, {"4", "8"},
                {"6", "6"},  {"6", "10"},
                {"9", "10"}, {"13", "14"} };
        REQUIRE(expected.size() == actual.size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(actual, expected));
    }

    SECTION("Test Both Invalid Arguments") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(17);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        StmtNoArgs args = StmtNoArgs();
        args.setStartStmtNo(-1);
        args.setEndStmtNo(500);
        vector<string> actual = designExtractor.getAffectsRelations(args);
        vector<string> expected = {};
        REQUIRE(actual == expected);
    }

    SECTION("Test Both Valid Arguments In If Procedure") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(17);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        StmtNoArgs args = StmtNoArgs();
        args.setStartStmtNo(1);
        args.setEndStmtNo(5);
        vector<string> actual = designExtractor.getAffectsRelations(args);
        vector<string> expected = { "1", "5" };
        REQUIRE(actual == expected);
    }

    SECTION("Test Both Valid Arguments But Invalid Answer In If Procedure") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(17);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        StmtNoArgs args = StmtNoArgs();
        args.setStartStmtNo(5);
        args.setEndStmtNo(1);
        vector<string> actual = designExtractor.getAffectsRelations(args);
        vector<string> expected = {};
        REQUIRE(actual == expected);
    }

    SECTION("Test Both Valid Arguments With Calls") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(18);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        StmtNoArgs args = StmtNoArgs();
        args.setStartStmtNo(1);
        args.setEndStmtNo(3);
        vector<string> actual = designExtractor.getAffectsRelations(args);
        vector<string> expected = {"1", "3"};
        REQUIRE(actual == expected);
    }

    SECTION("Test Both Valid Arguments With Print") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(19);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        StmtNoArgs args = StmtNoArgs();
        args.setStartStmtNo(1);
        args.setEndStmtNo(3);
        vector<string> actual = designExtractor.getAffectsRelations(args);
        vector<string> expected = {"1", "3"};
        REQUIRE(actual == expected);
    }

    SECTION("Test Variable Modified In First Arguments Not Used In Second") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(16);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        StmtNoArgs args = StmtNoArgs();
        args.setStartStmtNo(6);
        args.setEndStmtNo(8);
        vector<string> actual = designExtractor.getAffectsRelations(args);
        vector<string> expected = {};
        REQUIRE(actual == expected);
    }

    SECTION("Test Start Argument Only In If Statement") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(19);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        StmtNoArgs args = StmtNoArgs();
        args.setStartStmtNo(1);
        vector<string> actual = designExtractor.getAffectsRelations(args);
        vector<string> expected = {"3"};
        REQUIRE(actual == expected);
    }

    SECTION("Test End Argument Only With Calls") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(18);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        StmtNoArgs args = StmtNoArgs();
        args.setEndStmtNo(3);
        vector<string> actual = designExtractor.getAffectsRelations(args);
        vector<string> expected = { "1" };
        REQUIRE(actual == expected);
    }

    SECTION("Test Both Valid Argument With Normal Procedure") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(21);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        StmtNoArgs args = StmtNoArgs();
        args.setStartStmtNo(6);
        args.setEndStmtNo(6);
        vector<string> actual = designExtractor.getAffectsRelations(args);
        vector<string> expected = { "6", "6"};
        REQUIRE(actual == expected);
    }

    SECTION("Test End Arguments Only With Normal Procedure") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(21);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        StmtNoArgs args = StmtNoArgs();
        args.setEndStmtNo(10);
        vector<string> actual = designExtractor.getAffectsRelations(args);
        vector<string> expected = { "1", "2", "4", "6", "8", "9"};
        REQUIRE(TestDE::DEUtils::vectorStringContainsSameElements(actual, expected));
    }

    SECTION("Test Start Arguments Only With Normal Procedure") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(21);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        StmtNoArgs args = StmtNoArgs();
        args.setStartStmtNo(2);
        vector<string> actual = designExtractor.getAffectsRelations(args);
        vector<string> expected = { "6", "10" };
        REQUIRE(TestDE::DEUtils::vectorStringContainsSameElements(actual, expected));
    }

    SECTION("Test Invalid Both Args With Normal Procedure") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(21);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        StmtNoArgs args = StmtNoArgs();
        args.setStartStmtNo(9);
        args.setEndStmtNo(11);
        vector<string> actual = designExtractor.getAffectsRelations(args);
        vector<string> expected = {};
        REQUIRE(TestDE::DEUtils::vectorStringContainsSameElements(actual, expected));
    }

    SECTION("Test No Infinity Loop In While Block") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(22);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        StmtNoArgs args = StmtNoArgs();
        args.setStartStmtNo(8);
        args.setEndStmtNo(1);
        vector<string> actual = designExtractor.getAffectsRelations(args);
        vector<string> expected = {};
        REQUIRE(TestDE::DEUtils::vectorStringContainsSameElements(actual, expected));
    }

    SECTION("Test Valid Both Args In Nested While Block") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(22);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        StmtNoArgs args = StmtNoArgs();
        args.setStartStmtNo(8);
        args.setEndStmtNo(7);
        vector<string> actual = designExtractor.getAffectsRelations(args);
        vector<string> expected = {"8", "7"};
        REQUIRE(actual == expected);
    }
}