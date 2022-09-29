//
// Created by hongshu wang on 29/9/22.
//

#include "catch.hpp"
#include "CFG/CFG.h"
#include "../design_extractor/Dummies.h"

using namespace std;
using namespace TestDE;

TEST_CASE("Test CFG") {
    SECTION ("procedure 1") {
        // empty procedure -> empty cfg
        auto programNode = TestDE::Dummies::getTestProgramNode(0);
        vector<shared_ptr<ProcedureNode>> procedureList = programNode->procedureList;
        shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
                ASTUtils::getNodePtrToLineNumMap(programNode);
        CFG cfg = CFG(*procedureList[0], stmtNumbers);
        REQUIRE(cfg.cfg->empty());
    }

    SECTION ("procedure 2") {
        // procedure with no nesting
        auto programNode = TestDE::Dummies::getTestProgramNode(1);
        vector<shared_ptr<ProcedureNode>> procedureList = programNode->procedureList;
        shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
                ASTUtils::getNodePtrToLineNumMap(programNode);
        CFG cfg = CFG(*procedureList[0], stmtNumbers);
        REQUIRE(cfg.cfg->size() == 3);
        auto s1 = cfg.cfg->find(1)->second;
        auto s2 = cfg.cfg->find(2)->second;
        auto s3 = cfg.cfg->find(3)->second;
        REQUIRE(s1.size() == 1);
        REQUIRE(s2.size() == 1);
        REQUIRE(s3.size() == 0);
        REQUIRE(s1.find(2) != s1.end());
        REQUIRE(s2.find(3) != s2.end());
    }

    SECTION ("procedure 3") {
        // procedure with if
        auto programNode = TestDE::Dummies::getTestProgramNode(2);
        vector<shared_ptr<ProcedureNode>> procedureList = programNode->procedureList;
        shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
                ASTUtils::getNodePtrToLineNumMap(programNode);
        CFG cfg = CFG(*procedureList[0], stmtNumbers);
        REQUIRE(cfg.cfg->size() == 7);
        auto s1 = cfg.cfg->find(1)->second;
        auto s2 = cfg.cfg->find(2)->second;
        auto s3 = cfg.cfg->find(3)->second;
        auto s4 = cfg.cfg->find(4)->second;
        auto s5 = cfg.cfg->find(5)->second;
        auto s6 = cfg.cfg->find(6)->second;
        auto s7 = cfg.cfg->find(7)->second;
        REQUIRE(s1.size() == 1);
        REQUIRE(s2.size() == 1);
        REQUIRE(s3.size() == 2);
        REQUIRE(s4.size() == 1);
        REQUIRE(s5.size() == 1);
        REQUIRE(s6.size() == 1);
        REQUIRE(s7.size() == 0);
        REQUIRE(s1.find(2) != s1.end());
        REQUIRE(s2.find(3) != s2.end());
        REQUIRE(s3.find(4) != s3.end());
        REQUIRE(s3.find(6) != s3.end());
        REQUIRE(s4.find(5) != s4.end());
        REQUIRE(s5.find(7) != s5.end());
        REQUIRE(s6.find(7) != s6.end());
    }

    SECTION ("procedure 7") {
        // procedure with while
        auto programNode = TestDE::Dummies::getTestProgramNode(14);
        vector<shared_ptr<ProcedureNode>> procedureList = programNode->procedureList;
        shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
                ASTUtils::getNodePtrToLineNumMap(programNode);
        CFG cfg = CFG(*procedureList[0], stmtNumbers);
        REQUIRE(cfg.cfg->size() == 4);
        auto s1 = cfg.cfg->find(1)->second;
        auto s2 = cfg.cfg->find(2)->second;
        auto s3 = cfg.cfg->find(3)->second;
        auto s4 = cfg.cfg->find(4)->second;
        REQUIRE(s1.size() == 1);
        REQUIRE(s2.size() == 2);
        REQUIRE(s3.size() == 1);
        REQUIRE(s4.size() == 0);
        REQUIRE(s1.find(2) != s1.end());
        REQUIRE(s2.find(3) != s2.end());
        REQUIRE(s2.find(4) != s2.end());
        REQUIRE(s3.find(2) != s3.end());
    }

    SECTION ("procedure 5") {
        // procedure with while and if
        auto programNode = TestDE::Dummies::getTestProgramNode(4);
        vector<shared_ptr<ProcedureNode>> procedureList = programNode->procedureList;
        shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
                ASTUtils::getNodePtrToLineNumMap(programNode);
        CFG cfg = CFG(*procedureList[0], stmtNumbers);
        REQUIRE(cfg.cfg->size() == 7);
        auto s1 = cfg.cfg->find(1)->second;
        auto s2 = cfg.cfg->find(2)->second;
        auto s3 = cfg.cfg->find(3)->second;
        auto s4 = cfg.cfg->find(4)->second;
        auto s5 = cfg.cfg->find(5)->second;
        auto s6 = cfg.cfg->find(6)->second;
        auto s7 = cfg.cfg->find(7)->second;
        REQUIRE(s1.size() == 1);
        REQUIRE(s2.size() == 2);
        REQUIRE(s3.size() == 1);
        REQUIRE(s4.size() == 2);
        REQUIRE(s5.size() == 1);
        REQUIRE(s6.size() == 1);
        REQUIRE(s7.size() == 0);
        REQUIRE(s1.find(2) != s1.end());
        REQUIRE(s2.find(3) != s2.end());
        REQUIRE(s2.find(7) != s2.end());
        REQUIRE(s3.find(4) != s3.end());
        REQUIRE(s4.find(5) != s4.end());
        REQUIRE(s4.find(6) != s4.end());
        REQUIRE(s5.find(2) != s5.end());
        REQUIRE(s6.find(2) != s6.end());
    }

    SECTION ("procedure 8") {
        // procedure with nested if
        auto programNode = TestDE::Dummies::getTestProgramNode(15);
        vector<shared_ptr<ProcedureNode>> procedureList = programNode->procedureList;
        shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
                ASTUtils::getNodePtrToLineNumMap(programNode);
        CFG cfg = CFG(*procedureList[0], stmtNumbers);
        REQUIRE(cfg.cfg->size() == 5);
        auto s1 = cfg.cfg->find(1)->second;
        auto s2 = cfg.cfg->find(2)->second;
        auto s3 = cfg.cfg->find(3)->second;
        auto s4 = cfg.cfg->find(4)->second;
        auto s5 = cfg.cfg->find(5)->second;
        REQUIRE(s1.size() == 2);
        REQUIRE(s2.size() == 2);
        REQUIRE(s3.size() == 0);
        REQUIRE(s4.size() == 0);
        REQUIRE(s5.size() == 0);
        REQUIRE(s1.find(2) != s1.end());
        REQUIRE(s1.find(5) != s1.end());
        REQUIRE(s2.find(3) != s2.end());
        REQUIRE(s2.find(4) != s2.end());
    }
}