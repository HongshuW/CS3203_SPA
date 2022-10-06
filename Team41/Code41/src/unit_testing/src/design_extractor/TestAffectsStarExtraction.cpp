//
// Created by Aaron on 4/10/22.
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

TEST_CASE("Test AffectsStar Extractor") {
    SECTION("Test No Arguments") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(20);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        StmtNoArgs args = StmtNoArgs();
        list<vector<string>> actual = designExtractor.getAllAffectsStarRelations();
        vector<vector<string>> expected = { {"1", "2"}, {"2", "3"}, {"1", "3"}};
        REQUIRE(expected.size() == actual.size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(actual, expected));
    }

    SECTION("Test No Arguments With Normal Procedure") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(21);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        StmtNoArgs args = StmtNoArgs();
        list<vector<string>> actual = designExtractor.getAllAffectsStarRelations();
        vector<vector<string>> expected = { 
            {"1", "4"}, {"1", "8"}, {"1", "10"}, {"1", "11"}, 
            {"1", "12"}, {"10", "11"}, {"10", "12"}, {"11", "12"}, 
            {"2", "10"}, {"2", "11"}, {"2", "12"}, 
            {"2", "6"}, {"6", "11"}, {"6", "12"}, 
            {"8", "10"}, {"8", "11"}, {"8", "12"}, 
            {"4", "4"},  {"4", "8"}, {"4", "10"}, {"4", "11"}, {"4", "12"}, 
            {"6", "6"},  {"6", "10"},
            {"9", "10"}, {"9", "11"}, {"9", "12"}, {"13", "14"}};
        REQUIRE(expected.size() == actual.size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(actual, expected));
    }

    SECTION("Test Invalid Arguments") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(20);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        StmtNoArgs args = StmtNoArgs();
        args.setStartStmtNo(1000);
        args.setEndStmtNo(-13);
        vector<string> actual = designExtractor.getAffectsStarRelations(args);
        vector<string> expected = {};
        REQUIRE(actual == expected);
    }

    SECTION("Test Both Valid Arguments With Simple Procedure") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(20);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        StmtNoArgs args = StmtNoArgs();
        args.setStartStmtNo(1);
        args.setEndStmtNo(3);
        vector<string> actual = designExtractor.getAffectsStarRelations(args);
        vector<string> expected = {"1", "3"};
        REQUIRE(actual == expected);
    }

    SECTION("Test Valid Start Arg Only With Simple Procedure") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(20);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        StmtNoArgs args = StmtNoArgs();
        args.setStartStmtNo(1);
        vector<string> actual = designExtractor.getAffectsStarRelations(args);
        vector<string> expected = { "2", "3" };
        REQUIRE(actual == expected);
    }

    SECTION("Test Valid End Arg Only With Simple Procedure") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(20);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        StmtNoArgs args = StmtNoArgs();
        args.setEndStmtNo(3);
        vector<string> actual = designExtractor.getAffectsStarRelations(args);
        vector<string> expected = { "1", "2" };
        REQUIRE(actual == expected);
    }

    SECTION("Test Valid Start And End Arg Only With Normal Procedure") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(21);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        StmtNoArgs args = StmtNoArgs();
        args.setStartStmtNo(1);
        args.setEndStmtNo(12);
        vector<string> actual = designExtractor.getAffectsStarRelations(args);
        vector<string> expected = { "1", "12" };
        REQUIRE(actual == expected);
    }

    SECTION("Test Valid Start Arg Only With Normal Procedure") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(21);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        StmtNoArgs args = StmtNoArgs();
        args.setStartStmtNo(1);
        vector<string> actual = designExtractor.getAffectsStarRelations(args);
        vector<string> expected = { "4", "8", "10", "11", "12"};
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
        vector<string> actual = designExtractor.getAffectsStarRelations(args);
        vector<string> expected = {};
        REQUIRE(TestDE::DEUtils::vectorStringContainsSameElements(actual, expected));
    }

    SECTION("Test No Infinity Loop In Empty While Block") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(23);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        StmtNoArgs args = StmtNoArgs();
        args.setStartStmtNo(1);
        args.setEndStmtNo(3);
        vector<string> actual = designExtractor.getAffectsStarRelations(args);
        vector<string> expected = {"1", "3"};
        REQUIRE(actual == expected);
    } 
}