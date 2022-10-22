//
// Created by Xingchen Lin on 22/10/22.
//

#include "../../../unit_testing/src/design_extractor/DEUtils.h"
#include "catch.hpp"
#include "design_extractor/DesignExtractor.h"
#include "design_extractor/UsesExtractor.h"
#include "parser/ASTValidator.h"
#include "parser/Parser.h"
#include "parser/SPExceptions.h"
#include "parser/Tokenizer.h"
#include "pkb/DataModifier.h"

using namespace SourceParser;
using namespace std;
using namespace DE;

TEST_CASE("Test Uses Integration") {
  SECTION(
      "Test Parser + UseS Extraction -> basic source program with two "
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
    DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
    auto actual = UsesExtractor::extractUsesS(root);
    vector<vector<string>> expected = {{"1", "x"}, {"2", "y"}, {"2", "z"}};
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION(
      "Test Parser + UseS Extraction -> basic source program with if "
      "statements") {
    string source_2 =
        "procedure testProcedure {"
        "            x = 0;"
        "            if (x <= 1) then {"
        "            x = 1;"
        "            y = z + 1;"
        "            } else {"
        "            x = 5;"
        "            }"
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
    DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
    auto actual = UsesExtractor::extractUsesS(root);
    vector<vector<string>> expected = {{"2", "x"}, {"2", "z"}, {"4", "z"}};
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION(
      "Test Parser + UseS Extraction -> basic source program with nested "
      "statements") {
    string source_3 =
        "procedure testProcedure {"
        "            a = 3;"
        "            if (x <= 100) then {"
        "            while (x != 100) {"
        "            b = x + 1;"
        "            c = x + 3;"
        "            d = y + 4;"
        "            }"
        "            } else {"
        "            print e;"
        "            f = a + 3;"
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
    DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
    auto actual = UsesExtractor::extractUsesS(root);
    vector<vector<string>> expected = {
        {"2", "x"}, {"2", "y"}, {"2", "a"}, {"2", "e"}, {"3", "x"}, {"3", "y"},
        {"4", "x"}, {"5", "x"}, {"6", "y"}, {"7", "e"}, {"8", "a"}};
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }
}