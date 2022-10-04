//
// Created by Tan Wei En on 3/10/22.
//

#include "catch.hpp"

#include "InvalidProcedureDummies.h"
#include "parser/ASTValidator.h"
#include "parser/SPExceptions.h"
#include "../design_extractor/Dummies.h"

using namespace std;
using namespace SourceParser;

TEST_CASE("Test AST Validator") {
    SECTION("Test valid AST") {
        auto program = TestDE::Dummies::getTestProgramNode(0);
    }

    SECTION("Test invalid AST, procedure called does not exist") {
        auto program = TestAST::InvalidProcedureDummies::getTestProgramNode(0);
        ASTValidator astValidator = ASTValidator(program);
        REQUIRE_THROWS_AS(astValidator.validateAST(), SPASTException);
    }

    SECTION("Test invalid AST, procedure calls itself") {
        auto program = TestAST::InvalidProcedureDummies::getTestProgramNode(1);
        ASTValidator astValidator = ASTValidator(program);
        REQUIRE_THROWS_AS(astValidator.validateAST(), SPASTException);
    }

    SECTION("Test invalid AST, procedure calls itself in nested statement") {
        auto program = TestAST::InvalidProcedureDummies::getTestProgramNode(2);
        ASTValidator astValidator = ASTValidator(program);
        REQUIRE_THROWS_AS(astValidator.validateAST(), SPASTException);
    }

    SECTION("Test invalid AST, indirect cyclic dependency") {
        auto program = TestAST::InvalidProcedureDummies::getTestProgramNode(3);
        ASTValidator astValidator = ASTValidator(program);
        REQUIRE_THROWS_AS(astValidator.validateAST(), SPASTException);
    }

    SECTION("Test invalid AST, simple cyclic dependency") {
        auto program = TestAST::InvalidProcedureDummies::getTestProgramNode(4);
        ASTValidator astValidator = ASTValidator(program);
        REQUIRE_THROWS_AS(astValidator.validateAST(), SPASTException);
    }

    SECTION("Test invalid AST, called procedure name spelt with different casing") {
        auto program = TestAST::InvalidProcedureDummies::getTestProgramNode(5);
        ASTValidator astValidator = ASTValidator(program);
        REQUIRE_THROWS_AS(astValidator.validateAST(), SPASTException);
    }
}