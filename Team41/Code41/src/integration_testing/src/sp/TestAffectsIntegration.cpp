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

  SECTION(
      "Test AffectsT Extraction -> extract all AffectsT in deep nesting while "
      "loop") {
    string source_4 =
        "procedure deepNesting {\n"
        "    while (((x1 < 50) && (x1 >= 0)) || (x1 != 25)) {\n"
        "        while (x2 < x1) {\n"
        "            if (x3 != 0) then {\n"
        "                while ((x4 > x1) && (x4 >= x2))  {\n"
        "                    x5 = 5 * (5 + x1);\n"
        "                    x6 = x6 + x5 * 2 + x4;\n"
        "                    while (x7 >= 0) {\n"
        "                        while (x8 >= 0) {\n"
        "                            x9 = x9 + 1;\n"
        "                            if (x10 <= 9) then {\n"
        "                                print x11;\n"
        "                            } else {\n"
        "                                while (x12 < x10) {\n"
        "                                    print x13;\n"
        "                                    read x14;\n"
        "                                    while ((((x15 < x13) || (x11 < "
        "x10)) || (x1 < x2)) && (x1 > 0)) {\n"
        "                                        while (100 >= x16) {\n"
        "                                            x17 = x14 - 1;\n"
        "                                            print x18;\n"
        "                                        }\n"
        "                                    }\n"
        "                                }\n"
        "                            }\n"
        "                            x19 = x19 * x19;\n"
        "                            x20 = (x9 + x9) * 2;\n"
        "                            while (x21 < x20) {\n"
        "                                print x22;\n"
        "                            }\n"
        "                            call procedure29;\n"
        "                            x24 = 100;\n"
        "                        }\n"
        "                    }\n"
        "                }\n"
        "            } else {\n"
        "                x25 = 10;\n"
        "                print x26;\n"
        "            }\n"
        "            x27 = x2 - x1 + 10 / 2;\n"
        "        }\n"
        "        x28 = x18 / 2;\n"
        "    }\n"
        "}\n"
        "\n"
        "procedure procedure29 {\n"
        "    print x29;\n"
        "    x27 = x2 * 2 + x1 * 2 + 10;\n"
        "    read x28;\n"
        "    if (x32 >= 30) then {\n"
        "        x5 = x5 * 2;\n"
        "    } else {\n"
        "        x6 = x6 * 2;\n"
        "    }\n"
        "    call procedure35;\n"
        "}\n"
        "\n"
        "procedure procedure35 {\n"
        "    read x36;\n"
        "    while (x37 != x5) {\n"
        "        x17 = x17 * 2;\n"
        "    }\n"
        "    x1 = x2 + x3 + x4 + x5;\n"
        "    print x40;\n"
        "}\n"
        "\n"
        "procedure doesNotModify {\n"
        "    print x41;\n"
        "}";

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
    StmtNoArgs args = StmtNoArgs();
    args.setStartAndEndStmtNo(6, 6);
    vector<string> actual = designExtractor.getAffectsStarRelations(args);
    vector<string> expected = {"6", "6"};
    REQUIRE(actual == expected);
  }
}