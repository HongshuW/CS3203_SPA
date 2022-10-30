////
//// Created by Xingchen Lin on 22/10/22.
////
//
//#include "../../../unit_testing/src/design_extractor/DEUtils.h"
//#include "catch.hpp"
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
//TEST_CASE("Test Parent Integration") {
//  SECTION(
//      "Test Parser + Parent Extraction -> basic source program with two "
//      "statements") {
//    string source_1 =
//        "procedure testProcedure {"
//        "     print x;"
//        "     x = y + z;"
//        "}";
//
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
//    auto actual = ParentExtractor::extractParent(root);
//    vector<vector<string>> expected = {};
//    REQUIRE(expected.size() == actual->size());
//    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
//  }
//
//  SECTION(
//      "Test Parser + ParentT Extraction -> basic source program with two "
//      "statements") {
//    string source_1 =
//        "procedure testProcedure {"
//        "     print x;"
//        "     x = y + z;"
//        "}";
//
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
//    auto actual = ParentExtractor::extractParentT(root);
//    vector<vector<string>> expected = {};
//    REQUIRE(expected.size() == actual->size());
//    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
//  }
//
//  SECTION(
//      "Test Parser + Parent Extraction -> basic source program with if "
//      "statements") {
//    string source_2 =
//        "procedure testProcedure {"
//        "            x = 0;"
//        "            if (x <= 1) then {"
//        "            x = 1;"
//        "            y = z + 1;"
//        "            } else {"
//        "            x = 5;"
//        "            }"
//        "            }";
//
//    Tokenizer tokenizer = Tokenizer(source_2);
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
//    auto actual = ParentExtractor::extractParent(root);
//    vector<vector<string>> expected = {{"2", "3"}, {"2", "4"}, {"2", "5"}};
//    REQUIRE(expected.size() == actual->size());
//    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
//  }
//
//  SECTION(
//      "Test Parser + ParentT Extraction -> basic source program with if "
//      "statements") {
//    string source_2 =
//        "procedure testProcedure {"
//        "            x = 0;"
//        "            if (x <= 1) then {"
//        "            x = 1;"
//        "            y = z + 1;"
//        "            } else {"
//        "            x = 5;"
//        "            }"
//        "            }";
//
//    Tokenizer tokenizer = Tokenizer(source_2);
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
//    auto actual = ParentExtractor::extractParentT(root);
//    vector<vector<string>> expected = {{"2", "3"}, {"2", "4"}, {"2", "5"}};
//    REQUIRE(expected.size() == actual->size());
//    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
//  }
//
//  SECTION(
//      "Test Parser + Parent Extraction -> basic source program with nested "
//      "statements") {
//    string source_3 =
//        "procedure testProcedure {"
//        "            a = 3;"
//        "            if (x <= 100) then {"
//        "            while (x != 100) {"
//        "            b = x + 1;"
//        "            c = x + 3;"
//        "            d = y + 4;"
//        "            }"
//        "            } else {"
//        "            print e;"
//        "            f = a + 3;"
//        "            }"
//        "            }";
//
//    Tokenizer tokenizer = Tokenizer(source_3);
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
//    auto actual = ParentExtractor::extractParent(root);
//    vector<vector<string>> expected = {{"2", "3"}, {"2", "7"}, {"2", "8"},
//                                       {"3", "4"}, {"3", "5"}, {"3", "6"}};
//    REQUIRE(expected.size() == actual->size());
//    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
//  }
//
//  SECTION(
//      "Test Parser + ParentT Extraction -> basic source program with nested "
//      "statements") {
//    string source_3 =
//        "procedure testProcedure {"
//        "            a = 3;"
//        "            if (x <= 100) then {"
//        "            while (x != 100) {"
//        "            b = x + 1;"
//        "            c = x + 3;"
//        "            d = y + 4;"
//        "            }"
//        "            } else {"
//        "            print e;"
//        "            f = a + 3;"
//        "            }"
//        "            }";
//
//    Tokenizer tokenizer = Tokenizer(source_3);
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
//    auto actual = ParentExtractor::extractParentT(root);
//    vector<vector<string>> expected = {{"2", "3"}, {"2", "4"}, {"2", "5"},
//                                       {"2", "6"}, {"2", "7"}, {"2", "8"},
//                                       {"3", "4"}, {"3", "5"}, {"3", "6"}};
//    REQUIRE(expected.size() == actual->size());
//    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
//  }
//}