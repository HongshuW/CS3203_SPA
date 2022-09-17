#include "catch.hpp"
#include "iostream"
#include "DEUtils.h"
#include "parser/ASTBuilder.h"
#include "parser/Parser.h"
#include "parser/Tokenizer.h"
#include "parser/SPExceptions.h"
#include "parser/Parser.h"
#include "design_extractor/DesignExtractor.h"
#include "pkb/DataModifier.h"
#include "query_builder/QueryBuilder.h"

using namespace SourceParser;
using SourceParser::Tokenizer;
using namespace std;
using namespace DE;

TEST_CASE("Test Parser And Parent Extraction") {
	SECTION("Test Parser + Parent Extraction -> basic source program with two statements") {
        string source_1 = "../../../src/integration_testing/src/sp/parent_source/source1.txt";
        ASTBuilder astBuilder = ASTBuilder();
        shared_ptr<ProgramNode> root = astBuilder.buildAST(source_1);

        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
		QB::RelationType parent = RelationType::PARENT;
		auto actual = designExtractor.extractRelations(parent);
		vector<vector<string>> expected = { };
		REQUIRE(expected.size() == actual->size());
		REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
	}

	SECTION("Test Parser + ParentT Extraction -> basic source program with two statements") {
        string source_1 = "../../../src/integration_testing/src/sp/parent_source/source1.txt";
        ASTBuilder astBuilder = ASTBuilder();
        shared_ptr<ProgramNode> root = astBuilder.buildAST(source_1);

        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
		QB::RelationType parentT = RelationType::PARENT_T;
		auto actual = designExtractor.extractRelations(parentT);
		vector<vector<string>> expected = { };
		REQUIRE(expected.size() == actual->size());
		REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
	}

	SECTION("Test Parser + Parent Extraction -> basic source program with if statements") {
        string source_2 = "../../../src/integration_testing/src/sp/parent_source/source2.txt";
        ASTBuilder astBuilder = ASTBuilder();
        shared_ptr<ProgramNode> root = astBuilder.buildAST(source_2);

        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
		QB::RelationType parent = RelationType::PARENT;
		auto actual = designExtractor.extractRelations(parent);
		vector<vector<string>> expected = { {"2", "3"}, {"2", "4"}, {"2", "5"} };
		REQUIRE(expected.size() == actual->size());
		REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
	}

	SECTION("Test Parser + ParentT Extraction -> basic source program with if statements") {
        string source_2 = "../../../src/integration_testing/src/sp/parent_source/source2.txt";
        ASTBuilder astBuilder = ASTBuilder();
        shared_ptr<ProgramNode> root = astBuilder.buildAST(source_2);

        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
		QB::RelationType parentT = RelationType::PARENT_T;
		auto actual = designExtractor.extractRelations(parentT);
		vector<vector<string>> expected = { {"2", "3"}, {"2", "4"}, {"2", "5"} };
		REQUIRE(expected.size() == actual->size());
		REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
	}

	SECTION("Test Parser + Parent Extraction -> basic source program with nested statements") {
        string source_3 = "../../../src/integration_testing/src/sp/parent_source/source3.txt";
        ASTBuilder astBuilder = ASTBuilder();
        shared_ptr<ProgramNode> root = astBuilder.buildAST(source_3);

        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
		QB::RelationType parent = RelationType::PARENT;
		auto actual = designExtractor.extractRelations(parent);
		vector<vector<string>> expected = { {"2", "3"}, {"2", "7"}, {"2", "8"},
			{"3", "4"}, {"3", "5"}, {"3", "6"}};
		REQUIRE(expected.size() == actual->size());
		REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
	}

	SECTION("Test Parser + ParentT Extraction -> basic source program with nested statements") {
        string source_3 = "../../../src/integration_testing/src/sp/parent_source/source3.txt";
        ASTBuilder astBuilder = ASTBuilder();
        shared_ptr<ProgramNode> root = astBuilder.buildAST(source_3);

        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
		QB::RelationType parentT = RelationType::PARENT_T;
		auto actual = designExtractor.extractRelations(parentT);
		vector<vector<string>> expected = { {"2", "3"}, {"2", "4"}, {"2", "5"}, {"2", "6"}, {"2", "7"}, {"2", "8"},
			{"3", "4"}, {"3", "5"}, {"3", "6"} };
		REQUIRE(expected.size() == actual->size());
		REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
	}
}