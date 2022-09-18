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

TEST_CASE("Test Parser And UsesS Extraction") {
	SECTION("Test Parser + UseS Extraction -> basic source program with two statements") {
        string source_1 = "../../../src/integration_testing/src/sp/uses_source/source1.txt";
        ASTBuilder astBuilder = ASTBuilder();
        shared_ptr<ProgramNode> root = astBuilder.buildAST(source_1);

        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
		QB::RelationType usesS = RelationType::USES_S;
		auto actual = designExtractor.extractRelations(usesS);
		vector<vector<string>> expected = { {"1", "x"}, {"2", "y"}, {"2", "z"} };
		REQUIRE(expected.size() == actual->size());
		REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
	}

	SECTION("Test Parser + UseS Extraction -> basic source program with if statements") {
        string source_2 = "../../../src/integration_testing/src/sp/uses_source/source2.txt";
        ASTBuilder astBuilder = ASTBuilder();
        shared_ptr<ProgramNode> root = astBuilder.buildAST(source_2);

        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
		QB::RelationType usesS = RelationType::USES_S;
		auto actual = designExtractor.extractRelations(usesS);
		vector<vector<string>> expected = { {"2", "x"}, {"2", "z"}, {"4", "z"}};
		REQUIRE(expected.size() == actual->size());
		REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
	}

	SECTION("Test Parser + UseS Extraction -> basic source program with nested statements") {
        string source_3 = "../../../src/integration_testing/src/sp/uses_source/source3.txt";
        ASTBuilder astBuilder = ASTBuilder();
        shared_ptr<ProgramNode> root = astBuilder.buildAST(source_3);

        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, root);

		QB::RelationType usesS = RelationType::USES_S;
		auto actual = designExtractor.extractRelations(usesS);
		vector<vector<string>> expected = { {"2", "x"}, {"2", "y"}, {"2", "a"}, {"2", "e"}, {"3", "x"},
			{"3", "y"}, {"4", "x"}, {"5", "x"}, {"6", "y"}, {"7", "e"}, {"8", "a"}};
		REQUIRE(expected.size() == actual->size());
		REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
	}
}