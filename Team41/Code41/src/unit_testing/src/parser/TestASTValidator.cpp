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
    SECTION("Test valid AST 1") {
        auto program = TestDE::Dummies::getTestProgramNode(1);
        ASTValidator astValidator = ASTValidator(program);
        REQUIRE_NOTHROW(astValidator.validateAST());
    }

    SECTION("Test valid AST 2") {
        auto program = TestDE::Dummies::getTestProgramNode(2);
        ASTValidator astValidator = ASTValidator(program);
        REQUIRE_NOTHROW(astValidator.validateAST());
    }

    SECTION("Test valid AST 3") {
        auto program = TestDE::Dummies::getTestProgramNode(3);
        ASTValidator astValidator = ASTValidator(program);
        REQUIRE_NOTHROW(astValidator.validateAST());
    }

    SECTION("Test valid AST 4") {
        auto program = TestDE::Dummies::getTestProgramNode(4);
        ASTValidator astValidator = ASTValidator(program);
        REQUIRE_NOTHROW(astValidator.validateAST());
    }

    SECTION("Test valid AST 5") {
        auto program = TestDE::Dummies::getTestProgramNode(5);
        ASTValidator astValidator = ASTValidator(program);
        REQUIRE_NOTHROW(astValidator.validateAST());
    }

    SECTION("Test valid AST 6") {
        auto program = TestDE::Dummies::getTestProgramNode(6);
        ASTValidator astValidator = ASTValidator(program);
        REQUIRE_NOTHROW(astValidator.validateAST());
    }

    SECTION("Test valid AST 7") {
        auto program = TestDE::Dummies::getTestProgramNode(7);
        ASTValidator astValidator = ASTValidator(program);
        REQUIRE_NOTHROW(astValidator.validateAST());
    }

    SECTION("Test valid AST 8") {
        auto program = TestDE::Dummies::getTestProgramNode(8);
        ASTValidator astValidator = ASTValidator(program);
        REQUIRE_NOTHROW(astValidator.validateAST());
    }

    SECTION("Test valid AST 9") {
        auto program = TestDE::Dummies::getTestProgramNode(9);
        ASTValidator astValidator = ASTValidator(program);
        REQUIRE_NOTHROW(astValidator.validateAST());
    }

    SECTION("Test valid AST 10") {
        auto program = TestDE::Dummies::getTestProgramNode(10);
        ASTValidator astValidator = ASTValidator(program);
        REQUIRE_NOTHROW(astValidator.validateAST());
    }

    SECTION("Test invalid AST, procedure called does not exist") {
        auto program = TestAST::InvalidProcedureDummies::getTestProgramNode(0);
        ASTValidator astValidator = ASTValidator(program);
        REQUIRE_THROWS_AS(astValidator.validateAST(), SPValidationException);
    }

    SECTION("Test invalid AST, procedure calls itself") {
        auto program = TestAST::InvalidProcedureDummies::getTestProgramNode(1);
        ASTValidator astValidator = ASTValidator(program);
        REQUIRE_THROWS_AS(astValidator.validateAST(), SPValidationException);
    }

    SECTION("Test invalid AST, procedure calls itself in nested statement") {
        auto program = TestAST::InvalidProcedureDummies::getTestProgramNode(2);
        ASTValidator astValidator = ASTValidator(program);
        REQUIRE_THROWS_AS(astValidator.validateAST(), SPValidationException);
    }

    SECTION("Test invalid AST, indirect cyclic dependency") {
        auto program = TestAST::InvalidProcedureDummies::getTestProgramNode(3);
        ASTValidator astValidator = ASTValidator(program);
        REQUIRE_THROWS_AS(astValidator.validateAST(), SPValidationException);
    }

    SECTION("Test invalid AST, simple cyclic dependency") {
        auto program = TestAST::InvalidProcedureDummies::getTestProgramNode(4);
        ASTValidator astValidator = ASTValidator(program);
        REQUIRE_THROWS_AS(astValidator.validateAST(), SPValidationException);
    }

    SECTION("Test invalid AST, called procedure name spelt with different casing") {
        auto program = TestAST::InvalidProcedureDummies::getTestProgramNode(5);
        ASTValidator astValidator = ASTValidator(program);
        REQUIRE_THROWS_AS(astValidator.validateAST(), SPValidationException);
    }
}