//
// Created by Tan Wei En on 3/10/22.
//

#include "ASTValidator.h"

#include <utility>
#include "SPExceptions.h"
#include "queue"
#include "AST/utils/ASTUtils.h"
#include "parser/constants/ValidatorConstants.h"

using namespace SourceParser;

ASTValidator::ASTValidator(shared_ptr<ProgramNode> ast): ast(std::move(ast)) {}

void ASTValidator::validateAST() {
    validateProcedureNames();
    validateNodes();
    validateCyclicDependencies();
}

void ASTValidator::validateProcedureNames() {
    for (const auto& procedure : ast->procedureList) {
        string procedureName = procedure->procedureName;
        auto iterator = procedureNames.find(procedureName);
        if (iterator != procedureNames.end()) {
            string errorMessage = ErrorMessageFormatter::formatErrorMessage(
                    ValidatorConstants::SP_AST_EXCEPTION_DUPLICATE_PROCEDURE, procedureName);
            throw SPValidationException(errorMessage);
        }

        procedureNames.insert(procedureName);
        procedureCalls.insert({procedureName, unordered_set<string>()});
    }
}

void ASTValidator::validateNodes() {
    for (const auto& procedure: ast->procedureList) {
        string procedureName = procedure->procedureName;
        queue<shared_ptr<StmtNode>> queue;
        for (const auto& statement: procedure->stmtList) {
            queue.push(statement);
        }

        while (!queue.empty()) {
            auto stmtNode = queue.front();
            queue.pop();
            NodeType nodeType = ASTUtils::getNodeType(stmtNode);

            if (nodeType == AST::IF_NODE) {
                shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(stmtNode);
                for (const auto& childStmtNode: ifNode->ifStmtList) {
                    queue.push(childStmtNode);
                }
                for (const auto& childStmtNode: ifNode->elseStmtList) {
                    queue.push(childStmtNode);
                }
            }

            if (nodeType == AST::WHILE_NODE) {
                shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(stmtNode);
                for (const auto& childStmtNode: whileNode->stmtList) {
                    queue.push(childStmtNode);
                }
            }

            if (nodeType == AST::CALL_NODE) {
                shared_ptr<CallNode> callNode = dynamic_pointer_cast<CallNode>(stmtNode);
                validateCallNode(callNode, procedureName);
            }
        }
    }
}

void ASTValidator::validateCallNode(const shared_ptr<CallNode>& callNode, const string& procedureName) {
    string calledProcedure = callNode->procedureName;

    if (calledProcedure == procedureName) {
        string errorMessage = ErrorMessageFormatter::formatErrorMessage(
                ValidatorConstants::SP_AST_EXCEPTION_PROCEDURE_CALLS_ITSELF, procedureName);
        throw SPValidationException(errorMessage);
    }

    auto iterator = procedureNames.find(procedureName);
    if (iterator == procedureNames.end()) {
        string errorMessage = ErrorMessageFormatter::formatErrorMessage(
                ValidatorConstants::SP_AST_EXCEPTION_PROCEDURE_NOT_FOUND, procedureName);
        throw SPValidationException(errorMessage);
    }

    auto callsIterator = procedureCalls.find(procedureName);
    if (callsIterator == procedureCalls.end()) {
        string errorMessage = ErrorMessageFormatter::formatErrorMessage(
                ValidatorConstants::SP_AST_EXCEPTION_PROCEDURE_NOT_FOUND, procedureName);
        throw SPValidationException(errorMessage);
    }

    unordered_set<string> calledProcedures = callsIterator->second;
    calledProcedures.insert(calledProcedure);
    callsIterator->second = calledProcedures;
}


void ASTValidator::validateCyclicDependencies() {
    for (auto &unorderedMapIterator: procedureCalls) {
        string mainProcedure = unorderedMapIterator.first;
        unordered_set<string> proceduresCalled = unorderedMapIterator.second;
        for (auto &unorderedSetIterator : proceduresCalled) {
            const string& procedure = unorderedSetIterator;
            calls(procedure, mainProcedure);
        }
    }
}

void ASTValidator::calls(const string& procedure, string calledProcedure) {
    if (procedure == calledProcedure) {
        throw SPValidationException(ValidatorConstants::SP_AST_EXCEPTION_CYCLIC_DEPENDENCY);
    }

    auto callsIterator = procedureCalls.find(procedure);
    if (callsIterator == procedureCalls.end()) {
        string errorMessage = ErrorMessageFormatter::formatErrorMessage(
                ValidatorConstants::SP_AST_EXCEPTION_PROCEDURE_NOT_FOUND, procedure);
        throw SPValidationException(errorMessage);
    }
    unordered_set<string> calledProcedures = callsIterator->second;
    for (auto &unorderedSetIterator : calledProcedures) {
        return calls(unorderedSetIterator, calledProcedure);
    }
}
