//
// Created by Xingchen Lin on 22/10/22.
//

#include "../../../unit_testing/src/design_extractor/DEUtils.h"
#include "catch.hpp"
#include "design_extractor/extractors/FollowsRelationExtractor.h"
#include "design_extractor/extractors/FollowsTRelationExtractor.h"
#include "parser/ASTValidator.h"
#include "parser/Parser.h"
#include "parser/SPExceptions.h"
#include "parser/Tokenizer.h"
#include "pkb/DataModifier.h"

using namespace SourceParser;
using namespace std;
using namespace DE;

TEST_CASE("Test Follows Integration") {
  SECTION(
      "Test Parser + Follows Extraction -> basic source program with two "
      "statements") {
    string source_1 =
        "procedure testProcedure {"
        "     print x;"
        "     x = y + z;"
        "}";

    Tokenizer tokenizer = Tokenizer(source_1);
    vector<string> tokens = tokenizer.tokenize();
    Parser parser = Parser(tokens);
    shared_ptr<ProgramNode> root = parser.parse();
    ASTValidator astValidator = ASTValidator(root);
    astValidator.validateAST();

    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    FollowsRelationExtractor followsExtractor =
        FollowsRelationExtractor(dataModifier, root);
    auto actual =
        static_pointer_cast<RelationResult>(followsExtractor.extract())
            ->getResult();
    vector<vector<string>> expected = {{"1", "2"}};
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION(
      "Test Parser + Follows Extraction -> basic source program with three "
      "statements") {
    string source_2 =
        "procedure testProcedure {"
        "            print x;"
        "            x = y + z;"
        "            z = x + 1;"
        "            }";

    Tokenizer tokenizer = Tokenizer(source_2);
    vector<string> tokens = tokenizer.tokenize();
    Parser parser = Parser(tokens);
    shared_ptr<ProgramNode> root = parser.parse();
    ASTValidator astValidator = ASTValidator(root);
    astValidator.validateAST();

    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    FollowsRelationExtractor followsExtractor =
        FollowsRelationExtractor(dataModifier, root);
    auto actual =
        static_pointer_cast<RelationResult>(followsExtractor.extract())
            ->getResult();
    vector<vector<string>> expected = {{"1", "2"}, {"2", "3"}};
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION(
      "Test Parser + FollowsT Extraction -> basic source program with three "
      "statement") {
    string source_2 =
        "procedure testProcedure {"
        "            print x;"
        "            x = y + z;"
        "            z = x + 1;"
        "            }";

    Tokenizer tokenizer = Tokenizer(source_2);
    vector<string> tokens = tokenizer.tokenize();
    Parser parser = Parser(tokens);
    shared_ptr<ProgramNode> root = parser.parse();
    ASTValidator astValidator = ASTValidator(root);
    astValidator.validateAST();

    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    FollowsTRelationExtractor followsTExtractor =
        FollowsTRelationExtractor(dataModifier, root);
    auto actual =
        static_pointer_cast<RelationResult>(followsTExtractor.extract())
            ->getResult();
    vector<vector<string>> expected = {{"1", "2"}, {"1", "3"}, {"2", "3"}};
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION(
      "Test Parser + Follows Extraction -> basic source program with one if "
      "statement") {
    string source_3 =
        "procedure testProcedure {"
        "            x = 0;"
        "            if (x <= 1) then {"
        "            x = 1;"
        "            } else {"
        "            x = 5;"
        "            }"
        "            }";

    Tokenizer tokenizer = Tokenizer(source_3);
    vector<string> tokens = tokenizer.tokenize();
    Parser parser = Parser(tokens);
    shared_ptr<ProgramNode> root = parser.parse();
    ASTValidator astValidator = ASTValidator(root);
    astValidator.validateAST();

    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    FollowsRelationExtractor followsExtractor =
        FollowsRelationExtractor(dataModifier, root);
    auto actual1 =
        static_pointer_cast<RelationResult>(followsExtractor.extract())
            ->getResult();
    vector<vector<string>> expected1 = {{"1", "2"}};
    REQUIRE(expected1.size() == actual1->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual1, expected1));
  }

  SECTION(
      "Test Parser + Follows Extraction -> basic source program with one if "
      "statement and two assignment") {
    string source_4 =
        "procedure testProcedure {"
        "            x = 0;"
        "            if (x <= 1) then {"
        "            x = 1;"
        "            y = 3;"
        "            } else {"
        "            x = 5;"
        "            }"
        "            }";

    Tokenizer tokenizer = Tokenizer(source_4);
    vector<string> tokens = tokenizer.tokenize();
    Parser parser = Parser(tokens);
    shared_ptr<ProgramNode> root = parser.parse();
    ASTValidator astValidator = ASTValidator(root);
    astValidator.validateAST();

    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    FollowsRelationExtractor followsExtractor =
        FollowsRelationExtractor(dataModifier, root);
    auto actual1 =
        static_pointer_cast<RelationResult>(followsExtractor.extract())
            ->getResult();
    vector<vector<string>> expected1 = {{"1", "2"}, {"3", "4"}};
    REQUIRE(expected1.size() == actual1->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual1, expected1));
  }

  SECTION(
      "Test Parser + FollowsT Extraction -> basic source program with if "
      "statement") {
    string source_5 =
        "procedure testProcedure {"
        "            x = 0;"
        "            if (x <= 1) then {"
        "            x = 1;"
        "            y = 3;"
        "            z = y + x;"
        "            } else {"
        "            x = 5;"
        "            }"
        "            }";

    Tokenizer tokenizer = Tokenizer(source_5);
    vector<string> tokens = tokenizer.tokenize();
    Parser parser = Parser(tokens);
    shared_ptr<ProgramNode> root = parser.parse();
    ASTValidator astValidator = ASTValidator(root);
    astValidator.validateAST();

    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    FollowsTRelationExtractor followsTExtractor =
        FollowsTRelationExtractor(dataModifier, root);
    auto actual1 =
        static_pointer_cast<RelationResult>(followsTExtractor.extract())
            ->getResult();
    vector<vector<string>> expected1 = {
        {"1", "2"}, {"3", "4"}, {"3", "5"}, {"4", "5"}};
    REQUIRE(expected1.size() == actual1->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual1, expected1));
  }

  SECTION(
      "Test Parser + Follows Extraction -> basic source program with multiple "
      "level of nesting") {
    string source_6 =
        "procedure testProcedure {"
        "            z = 3;"
        "            if (x <= 100) then {"
        "            while (x != 100) {"
        "            x = x + 1;"
        "            y = x + 3;"
        "            }"
        "            } else {"
        "            print x;"
        "            z = a + 3;"
        "            }"
        "            }";

    Tokenizer tokenizer = Tokenizer(source_6);
    vector<string> tokens = tokenizer.tokenize();
    Parser parser = Parser(tokens);
    shared_ptr<ProgramNode> root = parser.parse();
    ASTValidator astValidator = ASTValidator(root);
    astValidator.validateAST();

    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    FollowsRelationExtractor followsExtractor =
        FollowsRelationExtractor(dataModifier, root);
    auto actual1 =
        static_pointer_cast<RelationResult>(followsExtractor.extract())
            ->getResult();
    vector<vector<string>> expected1 = {{"1", "2"}, {"4", "5"}, {"6", "7"}};
    REQUIRE(expected1.size() == actual1->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual1, expected1));
  }

  SECTION(
      "Test Parser + FollowsT Extraction -> basic source program with multiple "
      "level of nesting") {
    string source_7 =
        "procedure testProcedure {"
        "            z = 3;"
        "            if (x <= 100) then {"
        "            while (x != 100) {"
        "            x = x + 1;"
        "            y = x + 3;"
        "            }"
        "            } else {"
        "            print x;"
        "            z = a + 3;"
        "            }"
        "            z = 5;"
        "            }";

    Tokenizer tokenizer = Tokenizer(source_7);
    vector<string> tokens = tokenizer.tokenize();
    Parser parser = Parser(tokens);
    shared_ptr<ProgramNode> root = parser.parse();
    ASTValidator astValidator = ASTValidator(root);
    astValidator.validateAST();

    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    FollowsTRelationExtractor followsTExtractor =
        FollowsTRelationExtractor(dataModifier, root);
    auto actual1 =
        static_pointer_cast<RelationResult>(followsTExtractor.extract())
            ->getResult();
    vector<vector<string>> expected1 = {
        {"1", "2"}, {"1", "8"}, {"2", "8"}, {"4", "5"}, {"6", "7"}};
    REQUIRE(expected1.size() == actual1->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual1, expected1));
  }
}