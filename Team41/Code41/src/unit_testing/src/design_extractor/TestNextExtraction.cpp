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
    SECTION("Test Invalid Start and End Arguments") {
        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
        auto programNode = TestDE::Dummies::getTestProgramNode(0);
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);
        NextStmtNoArgs args = NextStmtNoArgs();
        args.setStartStmtNo(100);
        args.setEndStmtNo(250);
        vector<string> expected = designExtractor.getNextRelations(args);
        REQUIRE(expected.empty());
    }
}