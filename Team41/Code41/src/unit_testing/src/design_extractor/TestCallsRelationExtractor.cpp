//
// Created by Xingchen Lin on 27/10/22.
//

#include <unordered_map>

#include "AST/IfNode.h"
#include "AST/PrintNode.h"
#include "DEUtils.h"
#include "Dummies.h"
#include "catch.hpp"
#include "design_extractor/extractors/CallsRelationExtractor.h"
#include "pkb/DataModifier.h"

using namespace std;
using namespace DE;

TEST_CASE("Test Calls Relation Extractor") {
	SECTION("Test Empty Procedure") {
		shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
		auto programNode = TestDE::Dummies::getTestProgramNode(0);
		shared_ptr<DataModifier> dataModifier =
						make_shared<DataModifier>(pkbStorage);
		CallsRelationExtractor callsExtractor =
						CallsRelationExtractor(dataModifier, programNode);
		auto actual = static_pointer_cast<RelationResult>(callsExtractor.extract())->getResult();
		REQUIRE(actual->empty());
	}

	SECTION("Test Call One Procedure") {
		shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
		auto programNode = TestDE::Dummies::getTestProgramNode(10);
		shared_ptr<DataModifier> dataModifier =
						make_shared<DataModifier>(pkbStorage);
		CallsRelationExtractor callsExtractor =
						CallsRelationExtractor(dataModifier, programNode);
		auto actual = static_pointer_cast<RelationResult>(callsExtractor.extract())->getResult();
		vector<vector<string>> expected = {{"procedure2", "procedure3", "2"}};
		REQUIRE(expected.size() == actual->size());
		REQUIRE(TestDE::DEUtils::containsSameElementTuple(*actual, expected));
	}

	SECTION("Test Two Procedures Call The Same Procedures") {
		shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
		auto programNode = TestDE::Dummies::getTestProgramNode(11);
		shared_ptr<DataModifier> dataModifier =
						make_shared<DataModifier>(pkbStorage);
		CallsRelationExtractor callsExtractor =
						CallsRelationExtractor(dataModifier, programNode);
		auto actual = static_pointer_cast<RelationResult>(callsExtractor.extract())->getResult();
		vector<vector<string>> expected = {{"procedure2", "procedure3", "2"},
																			 {"procedure2", "procedure4", "3"},
																			 {"procedure3", "procedure4", "7"}};
		REQUIRE(expected.size() == actual->size());
		REQUIRE(TestDE::DEUtils::containsSameElementTuple(*actual, expected));
	}

	SECTION("Test Call In Singly-Nested Procedure") {
		shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
		auto programNode = TestDE::Dummies::getTestProgramNode(12);
		shared_ptr<DataModifier> dataModifier =
						make_shared<DataModifier>(pkbStorage);
		CallsRelationExtractor callsExtractor =
						CallsRelationExtractor(dataModifier, programNode);
		auto actual = static_pointer_cast<RelationResult>(callsExtractor.extract())->getResult();
		vector<vector<string>> expected = {{"procedure2", "procedure3", "3"},
																			 {"procedure2", "procedure4", "5"},
																			 {"procedure3", "procedure4", "8"},
																			 {"procedure3", "procedure6", "9"}};
		REQUIRE(expected.size() == actual->size());
		REQUIRE(TestDE::DEUtils::containsSameElementTuple(*actual, expected));
	}

	SECTION("Test Call In Doubly-Nested Procedure") {
		shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
		auto programNode = TestDE::Dummies::getTestProgramNode(13);
		shared_ptr<DataModifier> dataModifier =
						make_shared<DataModifier>(pkbStorage);
		CallsRelationExtractor callsExtractor =
						CallsRelationExtractor(dataModifier, programNode);
		auto actual = static_pointer_cast<RelationResult>(callsExtractor.extract())->getResult();
		vector<vector<string>> expected = {{"procedure2", "procedure3", "4"},
																			 {"procedure2", "procedure4", "5"},
																			 {"procedure3", "procedure5", "9"},
																			 {"procedure4", "procedure6", "12"}};
		REQUIRE(expected.size() == actual->size());
		REQUIRE(TestDE::DEUtils::containsSameElementTuple(*actual, expected));
	}
}
