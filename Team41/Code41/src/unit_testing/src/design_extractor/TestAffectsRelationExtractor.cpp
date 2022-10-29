//
// Created by Xingchen Lin on 29/10/22.
//

#include <unordered_map>

#include "AST/IfNode.h"
#include "AST/PrintNode.h"
#include "DEUtils.h"
#include "Dummies.h"
#include "catch.hpp"
#include "design_extractor/extractors/AffectsRelationExtractor.h"
#include "parser/ASTValidator.h"
#include "parser/Parser.h"
#include "parser/Tokenizer.h"
#include "pkb/DataModifier.h"

using namespace std;
using namespace DE;

TEST_CASE("Test Affects Relation Extractor") {
  SECTION("Test No Arguments - procedure 18") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(17);
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    AffectsRelationExtractor affectsRelationExtractor =
        AffectsRelationExtractor(dataModifier, programNode);
    auto actual = static_pointer_cast<RelationResult>(
                      affectsRelationExtractor.extractAllRelations())
                      ->getResult();
    vector<vector<string>> expected = {{"1", "5"}, {"3", "5"}};
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("Test No Arguments - procedure 2") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(1);
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    AffectsRelationExtractor affectsRelationExtractor =
        AffectsRelationExtractor(dataModifier, programNode);
    auto actual = static_pointer_cast<RelationResult>(
                      affectsRelationExtractor.extractAllRelations())
                      ->getResult();
    vector<vector<string>> expected = {};
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("Test No Arguments - procedure 5") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(4);
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    AffectsRelationExtractor affectsRelationExtractor =
        AffectsRelationExtractor(dataModifier, programNode);
    auto actual = static_pointer_cast<RelationResult>(
                      affectsRelationExtractor.extractAllRelations())
                      ->getResult();
    vector<vector<string>> expected = {};
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("Test No Arguments") {
    /*
procedure p {
1.    x = a;
2.    while (x == 0) {
3.    y = x
4.      if (a == b) {
5.        z = b
}
else {
6.        a = c
}
7.      y = z
8.      z = v
}
}
*/

    string source_1 =
        "procedure p {"
        "     x = a;"
        "     while (x == 0) {"
        "     y = x;"
        "				if (a == b) then {"
        "					z = b;"
        "				}"
        "else {a = c;}"
        "y = z;"
        "     z = v;}"
        "}";

    Tokenizer tokenizer = Tokenizer(source_1);
    vector<string> tokens = tokenizer.tokenize();
    Parser parser = Parser(tokens);
    shared_ptr<ProgramNode> root = parser.parse();
    ASTValidator astValidator = ASTValidator(root);
    astValidator.validateAST();

    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    AffectsRelationExtractor affectsRelationExtractor =
        AffectsRelationExtractor(dataModifier, root);
    auto actual = static_pointer_cast<RelationResult>(
                      affectsRelationExtractor.extractAllRelations())
                      ->getResult();
    vector<vector<string>> expected = {{"1", "3"}, {"5", "7"}, {"8", "7"}};
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("Test No Arguments") {
    string source_1 =
        "procedure p {"
        "     x = a;"
        "     while (x == 0) {"
        "     y = x;"
        "				if (a == b) then {"
        "					z = b;"
        "				}"
        "else {a = a;}"
        "y = z;"
        "     z = v;}"
        "}";

    Tokenizer tokenizer = Tokenizer(source_1);
    vector<string> tokens = tokenizer.tokenize();
    Parser parser = Parser(tokens);
    shared_ptr<ProgramNode> root = parser.parse();
    ASTValidator astValidator = ASTValidator(root);
    astValidator.validateAST();

    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    AffectsRelationExtractor affectsRelationExtractor =
        AffectsRelationExtractor(dataModifier, root);
    auto actual = static_pointer_cast<RelationResult>(
                      affectsRelationExtractor.extractAllRelations())
                      ->getResult();
    vector<vector<string>> expected = {
        {"1", "3"}, {"5", "7"}, {"8", "7"}, {"6", "6"}};
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("Test No Arguments with Calls - procedure 22") {
    string source_1 =
        "procedure Second{"
        "x = 0;"
        "i = 5;"
        "while (i != 0) {"
        "x = x + 2 * y;"
        "call Third;"
        "i = i - 1;"
        "}"
        "if (x == 1) then {"
        "x = x + 1; }"
        "else {"
        "z = 1;"
        "}"
        "z = z + x + i;"
        "y = z + 2;"
        "x = x * y + z; }"
        "procedure Third{"
        "z = 5;"
        "x = z;"
        "print v; }";

    Tokenizer tokenizer = Tokenizer(source_1);
    vector<string> tokens = tokenizer.tokenize();
    Parser parser = Parser(tokens);
    shared_ptr<ProgramNode> root = parser.parse();
    ASTValidator astValidator = ASTValidator(root);
    astValidator.validateAST();

    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    AffectsRelationExtractor affectsRelationExtractor =
        AffectsRelationExtractor(dataModifier, root);
    auto actual = static_pointer_cast<RelationResult>(
                      affectsRelationExtractor.extractAllRelations())
                      ->getResult();
  }

  SECTION("Test No Arguments With Normal Procedure") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(21);
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    AffectsRelationExtractor affectsRelationExtractor =
        AffectsRelationExtractor(dataModifier, programNode);
    auto actual = static_pointer_cast<RelationResult>(
                      affectsRelationExtractor.extractAllRelations())
                      ->getResult();
    vector<vector<string>> expected = {
        {"1", "4"},   {"1", "8"},   {"1", "10"}, {"1", "12"}, {"10", "11"},
        {"10", "12"}, {"11", "12"}, {"2", "10"}, {"2", "6"},  {"8", "10"},
        {"8", "12"},  {"4", "4"},   {"4", "10"}, {"4", "12"}, {"4", "8"},
        {"6", "6"},   {"6", "10"},  {"9", "10"}, {"13", "14"}};
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  //	SECTION("Test Both Invalid Arguments") {
  //		shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
  //		auto programNode = TestDE::Dummies::getTestProgramNode(17);
  //		shared_ptr<DataModifier> dataModifier =
  //						make_shared<DataModifier>(pkbStorage);
  //		DesignExtractor designExtractor =
  //						DesignExtractor(dataModifier,
  // programNode); 		StmtNoArgs args = StmtNoArgs();
  // args.setStartStmtNo(-1); 		args.setEndStmtNo(500); 		vector<string> actual =
  // designExtractor.getAffectsRelations(args); 		vector<string> expected =
  // {}; 		REQUIRE(actual == expected);
  //	}
  //
  //	SECTION("Test Both Valid Arguments In If Procedure") {
  //		shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
  //		auto programNode = TestDE::Dummies::getTestProgramNode(17);
  //		shared_ptr<DataModifier> dataModifier =
  //						make_shared<DataModifier>(pkbStorage);
  //		DesignExtractor designExtractor =
  //						DesignExtractor(dataModifier,
  // programNode); 		StmtNoArgs args = StmtNoArgs();
  // args.setStartStmtNo(1); 		args.setEndStmtNo(5); 		vector<string> actual =
  // designExtractor.getAffectsRelations(args); 		vector<string> expected =
  // {"1", "5"}; 		REQUIRE(actual == expected);
  //	}
  //
  //	SECTION("Test Both Valid Arguments But Invalid Answer In If Procedure")
  //{ 		shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
  //auto programNode = TestDE::Dummies::getTestProgramNode(17);
  //		shared_ptr<DataModifier> dataModifier =
  //						make_shared<DataModifier>(pkbStorage);
  //		DesignExtractor designExtractor =
  //						DesignExtractor(dataModifier,
  // programNode); 		StmtNoArgs args = StmtNoArgs();
  // args.setStartStmtNo(5); 		args.setEndStmtNo(1); 		vector<string> actual =
  // designExtractor.getAffectsRelations(args); 		vector<string> expected =
  // {}; 		REQUIRE(actual == expected);
  //	}
  //
  //	SECTION("Test Both Valid Arguments With Calls") {
  //		shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
  //		auto programNode = TestDE::Dummies::getTestProgramNode(18);
  //		shared_ptr<DataModifier> dataModifier =
  //						make_shared<DataModifier>(pkbStorage);
  //		DesignExtractor designExtractor =
  //						DesignExtractor(dataModifier,
  // programNode); 		StmtNoArgs args = StmtNoArgs();
  // args.setStartStmtNo(1); 		args.setEndStmtNo(3); 		vector<string> actual =
  // designExtractor.getAffectsRelations(args); 		vector<string> expected =
  // {"1", "3"}; 		REQUIRE(actual == expected);
  //	}
  //
  //	SECTION("Test Both Valid Arguments With Print") {
  //		shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
  //		auto programNode = TestDE::Dummies::getTestProgramNode(19);
  //		shared_ptr<DataModifier> dataModifier =
  //						make_shared<DataModifier>(pkbStorage);
  //		DesignExtractor designExtractor =
  //						DesignExtractor(dataModifier,
  // programNode); 		StmtNoArgs args = StmtNoArgs();
  // args.setStartStmtNo(1); 		args.setEndStmtNo(3); 		vector<string> actual =
  // designExtractor.getAffectsRelations(args); 		vector<string> expected =
  // {"1", "3"}; 		REQUIRE(actual == expected);
  //	}
  //
  //	SECTION("Test Variable Modified In First Arguments Not Used In Second")
  //{ 		shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
  //auto programNode = TestDE::Dummies::getTestProgramNode(16);
  //		shared_ptr<DataModifier> dataModifier =
  //						make_shared<DataModifier>(pkbStorage);
  //		DesignExtractor designExtractor =
  //						DesignExtractor(dataModifier,
  // programNode); 		StmtNoArgs args = StmtNoArgs();
  // args.setStartStmtNo(6); 		args.setEndStmtNo(8); 		vector<string> actual =
  // designExtractor.getAffectsRelations(args); 		vector<string> expected =
  // {}; 		REQUIRE(actual == expected);
  //	}
  //
  //	SECTION("Test Start Argument Only In If Statement") {
  //		shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
  //		auto programNode = TestDE::Dummies::getTestProgramNode(19);
  //		shared_ptr<DataModifier> dataModifier =
  //						make_shared<DataModifier>(pkbStorage);
  //		DesignExtractor designExtractor =
  //						DesignExtractor(dataModifier,
  // programNode); 		StmtNoArgs args = StmtNoArgs();
  // args.setStartStmtNo(1); 		vector<string> actual =
  // designExtractor.getAffectsRelations(args); 		vector<string> expected =
  // {"3"}; 		REQUIRE(actual == expected);
  //	}
  //
  //	SECTION("Test End Argument Only With Calls") {
  //		shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
  //		auto programNode = TestDE::Dummies::getTestProgramNode(18);
  //		shared_ptr<DataModifier> dataModifier =
  //						make_shared<DataModifier>(pkbStorage);
  //		DesignExtractor designExtractor =
  //						DesignExtractor(dataModifier,
  // programNode); 		StmtNoArgs args = StmtNoArgs();
  // args.setEndStmtNo(3); 		vector<string> actual =
  // designExtractor.getAffectsRelations(args); 		vector<string> expected =
  // {"1"}; 		REQUIRE(actual == expected);
  //	}
  //
  //	SECTION("Test Both Valid Argument With Normal Procedure") {
  //		shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
  //		auto programNode = TestDE::Dummies::getTestProgramNode(21);
  //		shared_ptr<DataModifier> dataModifier =
  //						make_shared<DataModifier>(pkbStorage);
  //		DesignExtractor designExtractor =
  //						DesignExtractor(dataModifier,
  // programNode); 		StmtNoArgs args = StmtNoArgs();
  // args.setStartStmtNo(6); 		args.setEndStmtNo(6); 		vector<string> actual =
  // designExtractor.getAffectsRelations(args); 		vector<string> expected =
  // {"6", "6"}; 		REQUIRE(actual == expected);
  //	}
  //
  //	SECTION("Test End Arguments Only With Normal Procedure") {
  //		shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
  //		auto programNode = TestDE::Dummies::getTestProgramNode(21);
  //		shared_ptr<DataModifier> dataModifier =
  //						make_shared<DataModifier>(pkbStorage);
  //		DesignExtractor designExtractor =
  //						DesignExtractor(dataModifier,
  // programNode); 		StmtNoArgs args = StmtNoArgs();
  // args.setEndStmtNo(10); 		vector<string> actual =
  // designExtractor.getAffectsRelations(args); 		vector<string> expected =
  // {"1", "2", "4", "6", "8", "9"}; 		REQUIRE(
  //						TestDE::DEUtils::vectorStringContainsSameElements(actual,
  // expected));
  //	}
  //
  //	SECTION("Test Start Arguments Only With Normal Procedure") {
  //		shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
  //		auto programNode = TestDE::Dummies::getTestProgramNode(21);
  //		shared_ptr<DataModifier> dataModifier =
  //						make_shared<DataModifier>(pkbStorage);
  //		DesignExtractor designExtractor =
  //						DesignExtractor(dataModifier,
  // programNode); 		StmtNoArgs args = StmtNoArgs();
  // args.setStartStmtNo(2); 		vector<string> actual =
  // designExtractor.getAffectsRelations(args); 		vector<string> expected =
  // {"6", "10"}; 		REQUIRE(
  //TestDE::DEUtils::vectorStringContainsSameElements(actual, expected));
  //	}
  //
  //	SECTION("Test Invalid Both Args With Normal Procedure") {
  //		shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
  //		auto programNode = TestDE::Dummies::getTestProgramNode(21);
  //		shared_ptr<DataModifier> dataModifier =
  //						make_shared<DataModifier>(pkbStorage);
  //		DesignExtractor designExtractor =
  //						DesignExtractor(dataModifier,
  // programNode); 		StmtNoArgs args = StmtNoArgs();
  // args.setStartStmtNo(9); 		args.setEndStmtNo(11); 		vector<string> actual =
  // designExtractor.getAffectsRelations(args); 		vector<string> expected =
  // {}; 		REQUIRE( 						TestDE::DEUtils::vectorStringContainsSameElements(actual,
  // expected));
  //	}
  //
  //	SECTION("Test No Infinity Loop In While Block") {
  //		shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
  //		auto programNode = TestDE::Dummies::getTestProgramNode(22);
  //		shared_ptr<DataModifier> dataModifier =
  //						make_shared<DataModifier>(pkbStorage);
  //		DesignExtractor designExtractor =
  //						DesignExtractor(dataModifier,
  // programNode); 		StmtNoArgs args = StmtNoArgs();
  // args.setStartStmtNo(8); 		args.setEndStmtNo(1); 		vector<string> actual =
  // designExtractor.getAffectsRelations(args); 		vector<string> expected =
  // {}; 		REQUIRE( 						TestDE::DEUtils::vectorStringContainsSameElements(actual,
  // expected));
  //	}
  //
  //	SECTION("Test Valid Both Args In Nested While Block") {
  //		shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
  //		auto programNode = TestDE::Dummies::getTestProgramNode(22);
  //		shared_ptr<DataModifier> dataModifier =
  //						make_shared<DataModifier>(pkbStorage);
  //		DesignExtractor designExtractor =
  //						DesignExtractor(dataModifier,
  // programNode); 		StmtNoArgs args = StmtNoArgs();
  // args.setStartStmtNo(8); 		args.setEndStmtNo(7); 		vector<string> actual =
  // designExtractor.getAffectsRelations(args); 		vector<string> expected =
  // {"8", "7"}; 		REQUIRE(actual == expected);
  //	}
}
