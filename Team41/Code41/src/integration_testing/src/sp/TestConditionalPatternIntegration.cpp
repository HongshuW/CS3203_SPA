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
//TEST_CASE("Test Conditional Pattern Integration") {
//  SECTION("Test Parser + Conditional Pattern Extraction -> all variable same") {
//    string source_1 =
//        "procedure testProcedure {\n"
//        "            x = 0;\n"
//        "            if ( (x <= x) && (x == x + x * x - x)) then {\n"
//        "            x = 1;\n"
//        "            y = 3;\n"
//        "            } else {\n"
//        "            x = 5;\n"
//        "            }\n"
//        "            }";
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
//    list<vector<string>> actual = PatternExtractor::extractIfPattern(root);
//    vector<vector<string>> expected = {{"2", "x"}};
//    REQUIRE(expected.size() == actual.size());
//    REQUIRE(TestDE::DEUtils::containsSameElementPair(actual, expected));
//  }
//
//  SECTION(
//      "Test Parser + If Conditional Pattern Extraction -> doubly nested "
//      "loops") {
//    string source_2 =
//        "procedure procedure2 {\n"
//        "   x = 5;\n"
//        "   if ((baz == qux + 1) && (5 + 3 < quux)) then {\n"
//        "        if ((bing == boom + 1) || (5 + 3 < qxxx)) then {\n"
//        "            x = 10;\n"
//        "        } else {\n"
//        "           y = x;\n"
//        "        }\n"
//        "   } else {\n"
//        "        x = 5;\n"
//        "   }\n"
//        "}";
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
//    list<vector<string>> actual = PatternExtractor::extractIfPattern(root);
//    vector<vector<string>> expected = {{"2", "baz"},  {"2", "qux"},
//                                       {"2", "quux"}, {"3", "bing"},
//                                       {"3", "boom"}, {"3", "qxxx"}};
//    REQUIRE(expected.size() == actual.size());
//    REQUIRE(TestDE::DEUtils::containsSameElementPair(actual, expected));
//  }
//
//  SECTION(
//      "Test Parser + While Conditional Pattern Extraction -> triple nested "
//      "loops") {
//    string source_3 =
//        "procedure procedure2 {\n"
//        "   x = 5;\n"
//        "   while ((baz == qux + 1) && (5 + 3 < quux)) {\n"
//        "        while ((bing == boom + 1) || (5 + 3 < qxxx)) {\n"
//        "            x = 10;\n"
//        "            while (y == x + 1) {\n"
//        "              y = 1000;\n"
//        "            }\n"
//        "        }\n"
//        "   }\n"
//        "}";
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
//    list<vector<string>> actual = PatternExtractor::extractWhilePattern(root);
//    vector<vector<string>> expected = {
//        {"2", "baz"},  {"2", "qux"},  {"2", "quux"}, {"3", "bing"},
//        {"3", "boom"}, {"3", "qxxx"}, {"5", "x"},    {"5", "y"}};
//    REQUIRE(expected.size() == actual.size());
//    REQUIRE(TestDE::DEUtils::containsSameElementPair(actual, expected));
//  }
//}
