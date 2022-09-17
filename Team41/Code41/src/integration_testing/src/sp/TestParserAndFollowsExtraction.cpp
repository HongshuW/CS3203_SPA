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
#include <filesystem>
#include <iostream>


using namespace SourceParser;
using SourceParser::Tokenizer;
using namespace std;
using namespace DE;

TEST_CASE("Test Parser And Follow Extraction") {
    SECTION("Test Parser + Follows Extraction -> basic source program with two statements") {
        string source_1 = "../../../src/integration_testing/src/sp/follows_source/source1.txt";
        ASTBuilder astBuilder = ASTBuilder();
        shared_ptr<ProgramNode> root = astBuilder.buildAST(source_1);

        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
        QB::RelationType follows = RelationType::FOLLOWS;
        auto actual = designExtractor.extractRelations(follows);
        vector<vector<string>> expected = { {"1", "2"} };
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }

    SECTION("Test Parser + Follows Extraction -> basic source program with three statements") {
        string source_2 = "../../../src/integration_testing/src/sp/follows_source/source2.txt";
        ASTBuilder astBuilder = ASTBuilder();
        shared_ptr<ProgramNode> root = astBuilder.buildAST(source_2);

        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
        QB::RelationType follows = RelationType::FOLLOWS;
        auto actual = designExtractor.extractRelations(follows);
        vector<vector<string>> expected = { {"1", "2"}, {"2", "3"}};
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }

    SECTION("Test Parser + FollowsT Extraction -> basic source program with three statement") {
        string source_2 = "../../../src/integration_testing/src/sp/follows_source/source2.txt";
        ASTBuilder astBuilder = ASTBuilder();
        shared_ptr<ProgramNode> root = astBuilder.buildAST(source_2);

        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
        QB::RelationType followsT = RelationType::FOLLOWS_T;
        auto actual = designExtractor.extractRelations(followsT);
        vector<vector<string>> expected = { {"1", "2"}, {"1", "3"}, {"2", "3"}};
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }

    SECTION("Test Parser + Follows Extraction -> basic source program with one if statement") {
        string source_3 = "../../../src/integration_testing/src/sp/follows_source/source3.txt";
        ASTBuilder astBuilder = ASTBuilder();
        shared_ptr<ProgramNode> root = astBuilder.buildAST(source_3);

        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
        QB::RelationType follows1 = RelationType::FOLLOWS;
        auto actual1 = designExtractor.extractRelations(follows1);
        vector<vector<string>> expected1 = { {"1", "2"}};
        REQUIRE(expected1.size() == actual1->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual1, expected1));
    }

    SECTION("Test Parser + Follows Extraction -> basic source program with one if statement and two assignment") {
        string source_4 = "../../../src/integration_testing/src/sp/follows_source/source4.txt";
        ASTBuilder astBuilder = ASTBuilder();
        shared_ptr<ProgramNode> root = astBuilder.buildAST(source_4);

        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
        QB::RelationType follows1 = RelationType::FOLLOWS;
        auto actual1 = designExtractor.extractRelations(follows1);
        vector<vector<string>> expected1 = { {"1", "2"}, {"3", "4"}};
        REQUIRE(expected1.size() == actual1->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual1, expected1));
    }

    SECTION("Test Parser + FollowsT Extraction -> basic source program with if statement") {
        string source_5 = "../../../src/integration_testing/src/sp/follows_source/source5.txt";
        ASTBuilder astBuilder = ASTBuilder();
        shared_ptr<ProgramNode> root = astBuilder.buildAST(source_5);

        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
        QB::RelationType followst1 = RelationType::FOLLOWS_T;
        auto actual1 = designExtractor.extractRelations(followst1);
        vector<vector<string>> expected1 = { {"1", "2"}, {"3", "4"}, {"3", "5"}, {"4", "5"}
    };
        REQUIRE(expected1.size() == actual1->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual1, expected1));
    }

    SECTION("Test Parser + Follows Extraction -> basic source program with multiple level of nesting") {
        string source_6 = "../../../src/integration_testing/src/sp/follows_source/source6.txt";
        ASTBuilder astBuilder = ASTBuilder();
        shared_ptr<ProgramNode> root = astBuilder.buildAST(source_6);

        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
        QB::RelationType follows1 = RelationType::FOLLOWS;
        auto actual1 = designExtractor.extractRelations(follows1);
        vector<vector<string>> expected1 = { {"1", "2"}, {"4", "5"}, {"6", "7"}
        };
        REQUIRE(expected1.size() == actual1->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual1, expected1));
    }

    SECTION("Test Parser + FollowsT Extraction -> basic source program with multiple level of nesting") {
        string source_7 = "../../../src/integration_testing/src/sp/follows_source/source7.txt";
        ASTBuilder astBuilder = ASTBuilder();
        shared_ptr<ProgramNode> root = astBuilder.buildAST(source_7);

        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
        QB::RelationType followst1 = RelationType::FOLLOWS_T;
        auto actual1 = designExtractor.extractRelations(followst1);
        vector<vector<string>> expected1 = { {"1", "2"}, {"1", "8"}, {"2", "8"}, {"4", "5"}, {"6", "7"}
        };
        REQUIRE(expected1.size() == actual1->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual1, expected1));
    }
}

