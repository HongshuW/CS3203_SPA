//
// Created by Xingchen Lin on 18/9/22.
//
#include "catch.hpp"
#include "../../../unit_testing/src/design_extractor/DEUtils.h"
#include "parser/ASTBuilder.h"
#include "parser/Parser.h"
#include "parser/Tokenizer.h"
#include "parser/SPExceptions.h"
#include "design_extractor/DesignExtractor.h"
#include "design_extractor/PatternExtractor.h"
#include "pkb/DataModifier.h"
#include "query_builder/QueryBuilder.h"


using namespace SourceParser;
using namespace std;
using namespace DE;


TEST_CASE("Test SP Integration") {
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

    SECTION("Test Parser + Call Extraction -> deep nested source program") {
        string source_1 = "../../../src/integration_testing/src/sp/calls_source/source1.txt";
        ASTBuilder astBuilder = ASTBuilder();
        shared_ptr<ProgramNode> root = astBuilder.buildAST(source_1);

        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, root);

        RelationType callS = RelationType::CALLS;
        auto actual = designExtractor.extractRelations(callS);
        vector<vector<string>> expected = { {"deepNesting", "x23", "23"} };
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementTuple(*actual, expected));
    }

    SECTION("Test Parser + CallStar Extraction -> deep nested source program") {
        string source_1 = "../../../src/integration_testing/src/sp/calls_source/source1.txt";
        ASTBuilder astBuilder = ASTBuilder();
        shared_ptr<ProgramNode> root = astBuilder.buildAST(source_1);

        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, root);

        QB::RelationType callT = RelationType::CALLS_T;
        auto actual = designExtractor.extractRelations(callT);
        vector<vector<string>> expected = { {"deepNesting", "x23"} };
        REQUIRE(expected.size() == actual->size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
    }

    SECTION("Test Parser + Conditional Pattern Extraction -> all variable same"){
        string source_1 = "../../../src/integration_testing/src/sp/conditional_pattern_source/source1.txt";
        ASTBuilder astBuilder = ASTBuilder();
        shared_ptr<ProgramNode> root = astBuilder.buildAST(source_1);

        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
        list<vector<string>> actual = designExtractor.extractIfPatterns();
        vector<vector<string>> expected = { {"2", "x"} };
        REQUIRE(expected.size() == actual.size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(actual, expected));
    }

    SECTION("Test Parser + If Conditional Pattern Extraction -> doubly nested loops"){
        string source_1 = "../../../src/integration_testing/src/sp/conditional_pattern_source/source2.txt";
        ASTBuilder astBuilder = ASTBuilder();
        shared_ptr<ProgramNode> root = astBuilder.buildAST(source_1);

        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
        list<vector<string>> actual = designExtractor.extractIfPatterns();
        vector<vector<string>> expected = {{"2", "baz"}, {"2", "qux"}, {"2", "quux"},
                                           {"3", "bing"}, {"3", "boom"}, {"3", "qxxx"}};
        REQUIRE(expected.size() == actual.size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(actual, expected));
    }

    SECTION("Test Parser + While Conditional Pattern Extraction -> triple nested loops"){
        string source_1 = "../../../src/integration_testing/src/sp/conditional_pattern_source/source3.txt";
        ASTBuilder astBuilder = ASTBuilder();
        shared_ptr<ProgramNode> root = astBuilder.buildAST(source_1);

        shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
        shared_ptr<DataModifier> dataModifier = make_shared<DataModifier>(pkbStorage);
        DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
        list<vector<string>> actual = designExtractor.extractWhilePatterns();
        vector<vector<string>> expected = {{"2", "baz"}, {"2", "qux"}, {"2", "quux"},
                                           {"3", "bing"}, {"3", "boom"}, {"3", "qxxx"},
                                           {"5", "x"}, {"5", "y"}};
        REQUIRE(expected.size() == actual.size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(actual, expected));
    }
}