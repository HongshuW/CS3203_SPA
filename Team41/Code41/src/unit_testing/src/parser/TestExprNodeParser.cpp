//
// Created by Xingchen Lin on 10/9/22.
//

#include "catch.hpp"
#include "utils/ExprNodeParser.h"

using namespace std;

TEST_CASE("Test SP ExprNode Parser") {
  SECTION("x + 1") {
    vector<string> tokens = vector<string>({"x", "+", "1"});
    ExprNodeParser parser = ExprNodeParser(tokens);
    shared_ptr<ExprNode> root = parser.parse();

    auto expectedRoot = make_shared<ExprNode>("+");
    expectedRoot->left = make_shared<ExprNode>("x");
    expectedRoot->right = make_shared<ExprNode>("1");
    REQUIRE(*root == *expectedRoot);
  }

  SECTION("z % 2") {
    vector<string> tokens = vector<string>({"z", "%", "2"});
    ExprNodeParser parser = ExprNodeParser(tokens);
    shared_ptr<ExprNode> root = parser.parse();

    auto expectedRoot = make_shared<ExprNode>("%");
    expectedRoot->left = make_shared<ExprNode>("z");
    expectedRoot->right = make_shared<ExprNode>("2");
    REQUIRE(*root == *expectedRoot);
  }

  SECTION("x * y") {
    vector<string> tokens = vector<string>({"x", "*", "y"});
    ExprNodeParser parser = ExprNodeParser(tokens);
    shared_ptr<ExprNode> root = parser.parse();

    auto expectedRoot = make_shared<ExprNode>("*");
    expectedRoot->left = make_shared<ExprNode>("x");
    expectedRoot->right = make_shared<ExprNode>("y");
    REQUIRE(*root == *expectedRoot);
  }

  SECTION("x + y + z") {
    vector<string> tokens = vector<string>({"x", "+", "y", "+", "z"});
    ExprNodeParser parser = ExprNodeParser(tokens);
    shared_ptr<ExprNode> root = parser.parse();

    auto leftChild = make_shared<ExprNode>("+");
    leftChild->left = make_shared<ExprNode>("x");
    leftChild->right = make_shared<ExprNode>("y");
    auto expectedRoot = make_shared<ExprNode>("+");
    expectedRoot->left = leftChild;
    expectedRoot->right = make_shared<ExprNode>("z");
    REQUIRE(*root == *expectedRoot);
  }

  SECTION("x + z * 5") {
    vector<string> tokens = vector<string>({"x", "+", "z", "*", "5"});
    ExprNodeParser parser = ExprNodeParser(tokens);
    shared_ptr<ExprNode> root = parser.parse();

    auto rightChild = make_shared<ExprNode>("*");
    rightChild->left = make_shared<ExprNode>("z");
    rightChild->right = make_shared<ExprNode>("5");
    auto expectedRoot = make_shared<ExprNode>("+");
    expectedRoot->left = make_shared<ExprNode>("x");
    ;
    expectedRoot->right = rightChild;
    REQUIRE(*root == *expectedRoot);
  }

  SECTION("z * 5 + x") {
    vector<string> tokens = vector<string>({"z", "*", "5", "+", "x"});
    ExprNodeParser parser = ExprNodeParser(tokens);
    shared_ptr<ExprNode> root = parser.parse();

    auto leftChild = make_shared<ExprNode>("*");
    leftChild->left = make_shared<ExprNode>("z");
    leftChild->right = make_shared<ExprNode>("5");
    auto expectedRoot = make_shared<ExprNode>("+");
    expectedRoot->left = leftChild;
    expectedRoot->right = make_shared<ExprNode>("x");
    REQUIRE(*root == *expectedRoot);
  }

  SECTION("(x + z) % 5") {
    vector<string> tokens = vector<string>({"(", "x", "+", "z", ")", "%", "5"});
    ExprNodeParser parser = ExprNodeParser(tokens);
    shared_ptr<ExprNode> root = parser.parse();

    auto leftChild = make_shared<ExprNode>("+");
    leftChild->left = make_shared<ExprNode>("x");
    leftChild->right = make_shared<ExprNode>("z");
    auto expectedRoot = make_shared<ExprNode>("%");
    expectedRoot->left = leftChild;
    expectedRoot->right = make_shared<ExprNode>("5");
    REQUIRE(*root == *expectedRoot);
  }

  SECTION("2 * x - (y + 7) / (1 + z)") {
    vector<string> tokens =
        vector<string>({"2", "*", "x", "-", "(", "y", "+", "7", ")", "/", "(",
                        "1", "+", "z", ")"});
    ExprNodeParser parser = ExprNodeParser(tokens);
    shared_ptr<ExprNode> root = parser.parse();

    auto leftChild = make_shared<ExprNode>("*");
    leftChild->left = make_shared<ExprNode>("2");
    leftChild->right = make_shared<ExprNode>("x");

    auto leftLeafChild = make_shared<ExprNode>("+");
    leftLeafChild->left = make_shared<ExprNode>("y");
    leftLeafChild->right = make_shared<ExprNode>("7");

    auto rightLeafChild = make_shared<ExprNode>("+");
    rightLeafChild->left = make_shared<ExprNode>("1");
    rightLeafChild->right = make_shared<ExprNode>("z");

    auto rightChild = make_shared<ExprNode>("/");
    rightChild->left = leftLeafChild;
    rightChild->right = rightLeafChild;

    auto expectedRoot = make_shared<ExprNode>("-");
    expectedRoot->left = leftChild;
    expectedRoot->right = rightChild;
    REQUIRE(*root == *expectedRoot);
  }
}
