//
// Created by Aaron on 2/10/22.
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

TEST_CASE("Test Next Extractor") {
    SECTION("Test Empty Arguments") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(1);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        NextStmtNoArgs args = NextStmtNoArgs();
        vector<string> expected = designExtractor.getNextRelations(args);
        REQUIRE(expected.empty());
    }

    SECTION("Test Invalid Start and End Arguments") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(1);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        NextStmtNoArgs args = NextStmtNoArgs();
        args.setStartStmtNo(100);
        args.setEndStmtNo(250);
        vector<string> actual = designExtractor.getNextRelations(args);
        REQUIRE(actual.empty());
    }

    SECTION("Test Valid Start and End Arguments") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(1);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        NextStmtNoArgs args = NextStmtNoArgs();
        args.setStartStmtNo(1);
        args.setEndStmtNo(2);
        vector<string> actual = designExtractor.getNextRelations(args);
        vector<string> expected = { "1", "2" };
        REQUIRE(actual == expected);
    }
}