//
// Created by Aaron on 23/10/22.
//

#include "../../../unit_testing/src/design_extractor/DEUtils.h"
#include "catch.hpp"
#include "design_extractor/AffectsExtractor.h"
#include "design_extractor/DesignExtractor.h"
#include "design_extractor/PatternExtractor.h"
#include "parser/ASTValidator.h"
#include "parser/Parser.h"
#include "parser/SPExceptions.h"
#include "parser/Tokenizer.h"
#include "pkb/DataModifier.h"

using namespace SourceParser;
using namespace std;
using namespace DE;

TEST_CASE("Test Affects Integration") {
  SECTION(
      "Test AffectsT Extraction -> intermediate results with simple program") {
    string source_1 =
        "procedure testProcedure {"
        "     x = y + z;"
        "     y = x + z;"
        "     a = y + z;"
        "     x = a + z;"
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
    StmtNoArgs args = StmtNoArgs();
    args.setStartAndEndStmtNo(1, 4);
    vector<string> actual = designExtractor.getAffectsStarRelations(args);
    vector<string> expected = {"1", "4"};
    REQUIRE(actual == expected);
  }

  SECTION("Test AffectsT Extraction -> intermediate results in while loop") {
    string source_2 =
        "procedure testProcedure {"
        "            a = 3;"
        "            if (x <= 100) then {"
        "            while (x != 100) {"
        "            x = y + z;"
        "            y = x + z;"
        "            a = y + z;"
        "            x = a + z;"
        "            }"
        "            } else {"
        "            print e;"
        "            f = a + 3;"
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
    StmtNoArgs args = StmtNoArgs();
    args.setStartAndEndStmtNo(4, 7);
    vector<string> actual = designExtractor.getAffectsStarRelations(args);
    vector<string> expected = {"4", "7"};
    REQUIRE(actual == expected);
  }

  SECTION(
      "Test AffectsT Extraction -> multiple intermediate results in while "
      "loop") {
    string source_3 =
        "procedure testProcedure {"
        "            a = 3;"
        "            if (x <= 100) then {"
        "            while (x != 100) {"
        "            x = y + c;"
        "            y = x + z;"
        "            a = y + z;"
        "            x = a + z;"
        "            b = x + z;"
        "            d = b + z;"
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
    StmtNoArgs args = StmtNoArgs();
    args.setStartAndEndStmtNo(4, 9);
    vector<string> actual = designExtractor.getAffectsStarRelations(args);
    vector<string> expected = {"4", "9"};
    REQUIRE(actual == expected);
  }

  SECTION(
      "Test AffectsT Extraction -> extract all AffectsT in while "
      "loop") {
    string source_4 =
        "procedure testProcedure {"
        "            while (i != 100) {"
        "            x = x + 2 * y;"
        "            x = x + 2 * y;"
        "            x = x + 2 * y;"
        "            i = i - 1;"
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
    DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
    list<vector<string>> actual = designExtractor.getAllAffectsStarRelations();
    list<vector<string>> expected = {
        {"2", "2"}, {"2", "3"}, {"2", "4"}, {"3", "2"}, {"3", "3"},
        {"3", "4"}, {"4", "2"}, {"4", "3"}, {"4", "4"}, {"5", "5"}};
    REQUIRE(actual == expected);
  }
}