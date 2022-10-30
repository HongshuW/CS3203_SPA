////
//// Created by Xingchen Lin on 22/10/22.
////
//
//#include "../../../unit_testing/src/design_extractor/DEUtils.h"
//#include "catch.hpp"
//#include "parser/ASTBuilder.h"
//#include "parser/ASTValidator.h"
//#include "parser/Parser.h"
//#include "parser/SPExceptions.h"
//#include "parser/Tokenizer.h"
//#include "pkb/DataModifier.h"
//
//using namespace SourceParser;
//using namespace std;
//using namespace DE;
//
//TEST_CASE("Test Calls Integration") {
//  string source_1 =
//      "procedure deepNesting {\n"
//      "    while (((x1 < 50) && (x1 >= 0)) || (x1 != 25)) {\n"
//      "        while (x2 < x1) {\n"
//      "            if (x3 != 0) then {\n"
//      "                while ((x4 > x1) && (x4 >= x2))  {\n"
//      "                    x5 = 5 * (5 + x1);\n"
//      "                    x6 = x6 + x5 * 2 + x4;\n"
//      "                    while (x7 >= 0) {\n"
//      "                        while (x8 >= 0) {\n"
//      "                            x9 = x9 + 1;\n"
//      "                            if (x10 <= x9) then {\n"
//      "                                print x11;\n"
//      "                            } else {\n"
//      "                                while (x12 < x10) {\n"
//      "                                    print x13;\n"
//      "                                    read x14;\n"
//      "                                    while ((((x15 < x13) || (x11 < "
//      "x10)) || (x1 < x2)) && (x1 > 0)) {\n"
//      "                                        while (100 >= x16) {\n"
//      "                                            x17 = x14 - 1;\n"
//      "                                            print x18;\n"
//      "                                        }\n"
//      "                                    }\n"
//      "                                }\n"
//      "                            }\n"
//      "                            x19 = x19 * x19;\n"
//      "                            x20 = (x9 + x9) * 2;\n"
//      "                            while (x21 < x20) {\n"
//      "                                print x22;\n"
//      "                            }\n"
//      "                            call x23;\n"
//      "                            x24 = 100;\n"
//      "                        }\n"
//      "                    }\n"
//      "                }\n"
//      "            } else {\n"
//      "                x25 = 10;\n"
//      "                print x26;\n"
//      "            }\n"
//      "            x27 = x2 - x1 + 10 / 2;\n"
//      "        }\n"
//      "        x28 = x18 / 2;\n"
//      "    }\n"
//      "}\n"
//      "\n"
//      "procedure x23 {\n"
//      "    x = 5;\n"
//      "}";
//
//  SECTION("Test Parser + Call Extraction -> deep nested source program") {
//    Tokenizer tokenizer = Tokenizer(source_1);
//    vector<string> tokens = tokenizer.tokenize();
//    Parser parser = Parser(tokens);
//    shared_ptr<ProgramNode> root = parser.parse();
//    ASTValidator astValidator = ASTValidator(root);
//    astValidator.validateAST();
//
//    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
//    shared_ptr<DataModifier> dataModifier =
//        make_shared<DataModifier>(pkbStorage);
//    DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
//    auto actual = CallsExtractor::extractCalls(root);
//    vector<vector<string>> expected = {{"deepNesting", "x23", "23"}};
//    REQUIRE(expected.size() == actual->size());
//    REQUIRE(TestDE::DEUtils::containsSameElementTuple(*actual, expected));
//  }
//
//  SECTION("Test Parser + CallStar Extraction -> deep nested source program") {
//    Tokenizer tokenizer = Tokenizer(source_1);
//    vector<string> tokens = tokenizer.tokenize();
//    Parser parser = Parser(tokens);
//    shared_ptr<ProgramNode> root = parser.parse();
//    ASTValidator astValidator = ASTValidator(root);
//    astValidator.validateAST();
//
//    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
//    shared_ptr<DataModifier> dataModifier =
//        make_shared<DataModifier>(pkbStorage);
//    DesignExtractor designExtractor = DesignExtractor(dataModifier, root);
//    auto actual = CallsExtractor::extractCallsStar(root);
//    vector<vector<string>> expected = {{"deepNesting", "x23"}};
//    REQUIRE(expected.size() == actual->size());
//    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
//  }
//}
