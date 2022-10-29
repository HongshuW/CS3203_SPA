//
// Created by Xingchen Lin on 29/10/22.
//

#include "../../../unit_testing/src/design_extractor/DEUtils.h"
#include "catch.hpp"
#include "design_extractor/extractors/AffectsRelationExtractor.h"
#include "design_extractor/results/QueryTimeResult.h"
#include "parser/ASTValidator.h"
#include "parser/Parser.h"
#include "parser/SPExceptions.h"
#include "parser/Tokenizer.h"
#include "pkb/DataModifier.h"

using namespace SourceParser;
using namespace std;
using namespace DE;

TEST_CASE("Test SP-DE Integration") {
  SECTION("Test Simple Affects Extraction") {
    string source =
        "procedure procedure1 {\n"
        "    read x;\n"
        "    read y;\n"
        "    read z;\n"
        "    x = y + z;\n"
        "    a = x * 2;\n"
        "    if (cond < 10) then {\n"
        "        y = x + 10;\n"
        "        z = z * 2;\n"
        "        a = z;\n"
        "    } else {\n"
        "        b = y + z;\n"
        "        c = 250;\n"
        "    }\n"
        "    x = 20;\n"
        "    call procedure2;\n"
        "    y = 10;\n"
        "    z = x + y + z + a + b + c;\n"
        "    foo = y + z + b;\n"
        "    bar = 10;\n"
        "    baz = a + b + z;\n"
        "}\n"
        "\n"
        "procedure procedure2 {\n"
        "    x = 50;\n"
        "    call procedure3;\n"
        "}\n"
        "\n"
        "procedure procedure3 {\n"
        "    while (z < 10) {\n"
        "        foo = foo * 2;\n"
        "    }\n"
        "}";

    Tokenizer tokenizer = Tokenizer(source);
    vector<string> tokens = tokenizer.tokenize();
    Parser parser = Parser(tokens);
    shared_ptr<ProgramNode> root = parser.parse();
    ASTValidator astValidator = ASTValidator(root);
    astValidator.validateAST();

    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    AffectsRelationExtractor affectsRelationExtractor =
        AffectsRelationExtractor(dataModifier, root);
    auto actual = static_pointer_cast<RelationResult>(
                      affectsRelationExtractor.extractAllRelations())
                      ->getResult();
    vector<vector<string>> expected = {
        {"4", "5"},   {"4", "7"},   {"5", "15"},  {"5", "18"},  {"8", "9"},
        {"8", "15"},  {"9", "15"},  {"9", "18"},  {"10", "15"}, {"10", "16"},
        {"10", "18"}, {"11", "15"}, {"14", "15"}, {"14", "16"}, {"15", "16"},
        {"15", "18"}, {"22", "22"}};
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));

    // test no wildcards
    for (vector<string> pair : expected) {
        string first = pair[0];
        string second = pair[1];
        StmtNoArgs argsNoWildcard = StmtNoArgs();
        argsNoWildcard.setStartStmtNo(stoi(first));
        argsNoWildcard.setEndStmtNo(stoi(second));
        auto actualNoWildcard = *static_pointer_cast<QueryTimeResult>(
                affectsRelationExtractor.extract(argsNoWildcard))
                ->getResult();
        vector<string> expectedNoWildcard = {first, second};
        REQUIRE(actualNoWildcard == expectedNoWildcard);
    }

    // test one wildcard with given start
    unordered_map<string, vector<string>> givenStartMap = unordered_map<string, vector<string>>();
    for (vector<string> pair : expected) {
        string first = pair[0];
        string second = pair[1];
        if (givenStartMap.find(first) == givenStartMap.end()) {
            givenStartMap.insert({first, vector<string>()});
        }
        auto pointer = givenStartMap.find(first);
        pointer->second.push_back(second);
    }
    for (auto& testcase : givenStartMap) {
        string start = testcase.first;
        StmtNoArgs argsStartGiven = StmtNoArgs();
        argsStartGiven.setStartStmtNo(stoi(start));
        auto actualStartGiven = *static_pointer_cast<QueryTimeResult>(
                affectsRelationExtractor.extract(argsStartGiven))
                ->getResult();
        vector<string> expectedStartGiven = testcase.second;
        REQUIRE(TestDE::DEUtils::vectorStringContainsSameElements(actualStartGiven, expectedStartGiven));
    }

    // test one wildcard with given end
    unordered_map<string, vector<string>> givenEndMap = unordered_map<string, vector<string>>();
    for (vector<string> pair : expected) {
        string first = pair[0];
        string second = pair[1];
        if (givenEndMap.find(second) == givenEndMap.end()) {
            givenEndMap.insert({second, vector<string>()});
        }
        auto pointer = givenEndMap.find(second);
        pointer->second.push_back(first);
    }
    for (auto& testcase : givenEndMap) {
        string end = testcase.first;
        StmtNoArgs argsEndGiven = StmtNoArgs();
        argsEndGiven.setEndStmtNo(stoi(end));
        auto actualEndGiven = *static_pointer_cast<QueryTimeResult>(
                affectsRelationExtractor.extract(argsEndGiven))
                ->getResult();
        vector<string> expectedEndGiven = testcase.second;
        REQUIRE(TestDE::DEUtils::vectorStringContainsSameElements(actualEndGiven, expectedEndGiven));
    }
  }
}