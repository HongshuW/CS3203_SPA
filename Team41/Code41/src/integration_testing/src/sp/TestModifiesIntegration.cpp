//
// Created by Xingchen Lin on 22/10/22.
//

#include "../../../unit_testing/src/design_extractor/DEUtils.h"
#include "catch.hpp"
#include "design_extractor/DesignExtractor.h"
#include "design_extractor/ModifiesExtractor.h"
#include "parser/ASTValidator.h"
#include "parser/Parser.h"
#include "parser/SPExceptions.h"
#include "parser/Tokenizer.h"

using namespace SourceParser;
using namespace std;
using namespace DE;

TEST_CASE("Test Modifies Integration") {
  SECTION(
      "Test Parser + ModifiesS Extraction -> basic source program with two "
      "statements") {
    string source_1 =
        "procedure testProcedure {"
        "            print x;"
        "            x = y + z;"
        "            }";

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
    auto actual = ModifiesExtractor::extractModifiesS(root);
    vector<vector<string>> expected = {{"2", "x"}};
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION(
      "Test Parser + ModifiesS Extraction -> basic source program with "
      "multiple statements") {
    string source_2 =
        "procedure testProcedure {"
        "            print x;"
        "            x = y + z;"
        "            z = y + 4;"
        "            a = y + 4;"
        "            c = y + 4;"
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
    auto actual = ModifiesExtractor::extractModifiesS(root);
    vector<vector<string>> expected = {
        {"2", "x"}, {"3", "z"}, {"4", "a"}, {"5", "c"}};
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION(
      "Test Parser + ModifiesS Extraction -> basic source program with nested "
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
    auto actual = ModifiesExtractor::extractModifiesS(root);
    vector<vector<string>> expected = {
        {"1", "a"}, {"2", "b"}, {"2", "c"}, {"2", "d"}, {"2", "f"}, {"3", "b"},
        {"3", "c"}, {"3", "d"}, {"4", "b"}, {"5", "c"}, {"6", "d"}, {"8", "f"}};
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }
}
