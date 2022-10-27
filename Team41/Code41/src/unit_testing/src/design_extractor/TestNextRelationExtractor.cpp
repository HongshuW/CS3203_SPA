//
// Created by Xingchen Lin on 27/10/22.
//

#include <unordered_map>

#include "AST/IfNode.h"
#include "AST/PrintNode.h"
#include "DEUtils.h"
#include "Dummies.h"
#include "catch.hpp"
#include "design_extractor/extractors/NextRelationExtractor.h"
#include "pkb/DataModifier.h"

using namespace std;
using namespace DE;

TEST_CASE("Test Next Relation Extractor") {
	SECTION("Test Empty Procedure") {
		shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
		auto programNode = TestDE::Dummies::getTestProgramNode(0);
		shared_ptr<DataModifier> dataModifier =
						make_shared<DataModifier>(pkbStorage);
		NextRelationExtractor nextExtractor =
						NextRelationExtractor(dataModifier, programNode);
//		auto actual = static_pointer_cast<RelationResult>(nextExtractor.extract())->getResult();
//		vector<vector<string>> expected = {};
//		REQUIRE(expected.size() == actual->size());
//		REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
	}

//	SECTION("Test Procedure With Assign Only") {
//		shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
//		auto programNode = TestDE::Dummies::getTestProgramNode(1);
//		shared_ptr<DataModifier> dataModifier =
//						make_shared<DataModifier>(pkbStorage);
//		NextRelationExtractor nextExtractor =
//						NextRelationExtractor(dataModifier, programNode);
//		auto actual = static_pointer_cast<RelationResult>(nextExtractor.extract())->getResult();
//		vector<vector<string>> expected = {{"1", "2"}, {"2", "3"}};
//		REQUIRE(expected.size() == actual->size());
//		REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
//	}
//
//	SECTION("Test Procedure With If Statments") {
//		shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
//		auto programNode = TestDE::Dummies::getTestProgramNode(2);
//		shared_ptr<DataModifier> dataModifier =
//						make_shared<DataModifier>(pkbStorage);
//		NextRelationExtractor nextExtractor =
//						NextRelationExtractor(dataModifier, programNode);
//		auto actual = static_pointer_cast<RelationResult>(nextExtractor.extract())->getResult();
//		vector<vector<string>> expected = {{"1", "2"}, {"2", "3"}, {"3", "4"},
//																			 {"3", "6"}, {"4", "5"}, {"5", "7"},
//																			 {"6", "7"}};
//		REQUIRE(expected.size() == actual->size());
//		REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
//	}
//
//	SECTION("Test Procedure With While Statments") {
//		shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
//		auto programNode = TestDE::Dummies::getTestProgramNode(24);
//		shared_ptr<DataModifier> dataModifier =
//						make_shared<DataModifier>(pkbStorage);
//		NextRelationExtractor nextExtractor =
//						NextRelationExtractor(dataModifier, programNode);
//		auto actual = static_pointer_cast<RelationResult>(nextExtractor.extract())->getResult();
//		vector<vector<string>> expected = {
//						{"1", "2"}, {"2", "3"}, {"2", "4"}, {"3", "2"}};
//		REQUIRE(expected.size() == actual->size());
//		REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
//	}
//
//	SECTION("Test Procedure With Nested If Statments In While Loop") {
//		shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
//		auto programNode = TestDE::Dummies::getTestProgramNode(4);
//		shared_ptr<DataModifier> dataModifier =
//						make_shared<DataModifier>(pkbStorage);
//		NextRelationExtractor nextExtractor =
//						NextRelationExtractor(dataModifier, programNode);
//		auto actual = static_pointer_cast<RelationResult>(nextExtractor.extract())->getResult();
//		vector<vector<string>> expected = {{"1", "2"}, {"2", "3"}, {"2", "7"},
//																			 {"3", "4"}, {"4", "5"}, {"4", "6"},
//																			 {"5", "2"}, {"6", "2"}};
//		REQUIRE(expected.size() == actual->size());
//		REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
//	}
}