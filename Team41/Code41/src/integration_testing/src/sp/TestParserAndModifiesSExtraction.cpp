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

TEST_CASE("Test Parser And Modifies Extraction") {
    SECTION("Test Parser + ModifiesS Extraction -> basic source program with two statements") {
        string source_1 = "../../../src/integration_testing/src/sp/modifies_source/source1.txt";
        ASTBuilder astBuilder = ASTBuilder();
        shared_ptr<ProgramNode> root = astBuilder.buildAST(source_1);

        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
        QB::RelationType modifiesS = RelationType::MODIFIES_S;
        auto actual = designExtractor.extractRelations(modifiesS);
        vector<vector<string>> expected = { {"2", "x"} };
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }

    SECTION("Test Parser + ModifiesS Extraction -> basic source program with multiple statements") {
        string source_2 = "../../../src/integration_testing/src/sp/modifies_source/source2.txt";
        ASTBuilder astBuilder = ASTBuilder();
        shared_ptr<ProgramNode> root = astBuilder.buildAST(source_2);

        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
        QB::RelationType modifiesS = RelationType::MODIFIES_S;
        auto actual = designExtractor.extractRelations(modifiesS);
        vector<vector<string>> expected = { {"2", "x"}, {"3", "z"}, {"4", "a"}, {"5", "c"}};
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected)); 
    }

    SECTION("Test Parser + ModifiesS Extraction -> basic source program with nested statements") {
        string source_3 = "../../../src/integration_testing/src/sp/modifies_source/source3.txt";
        ASTBuilder astBuilder = ASTBuilder();
        shared_ptr<ProgramNode> root = astBuilder.buildAST(source_3);

        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
        QB::RelationType modifiesS = RelationType::MODIFIES_S;
        auto actual = designExtractor.extractRelations(modifiesS);
        vector<vector<string>> expected = { {"1", "a"}, {"2", "b"}, {"2", "c"}, {"2", "d"},
        {"2", "f"}, {"3", "b"}, {"3", "c"}, {"3", "d"}, {"4", "b"}, {"5", "c"}, {"6", "d"}, {"8", "f"}};
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }
}