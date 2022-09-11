//
// Created by Nafour on 11/9/22.
//

#include "PatternExtractor.h"
#include "AST/utils/ASTUtils.h"
namespace DE {

    vector<pair<pair<int, string>, std::shared_ptr<AssignNode>>>
    PatternExtractor::extractPattern(shared_ptr<ProgramNode> programNode) {
        shared_ptr<vector<pair<pair<int, string>, std::shared_ptr<AssignNode>>>> resultVec = make_shared<vector<pair<pair<int, string>, std::shared_ptr<AssignNode>>>>();
        auto stmtNumbers = ASTUtils::getNodePtrToLineNumMap(programNode);
        for (auto procedureNode: programNode->procedureList) {
            extractPatternHelper(procedureNode, resultVec, stmtNumbers);
        }
        return *resultVec;
    }

    void PatternExtractor::extractPatternHelper(shared_ptr<ASTNode> node,
                                                shared_ptr<vector<pair<pair<int, string>, std::shared_ptr<AssignNode>>>> resultVec, shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers) {
        NodeType nodeType = ASTUtils::getNodeType(node);

        if (nodeType == AST::PROCEDURE_NODE) {
            shared_ptr<ProcedureNode> procedureNode1 = dynamic_pointer_cast<ProcedureNode>(node);
            for (auto stmtNode: procedureNode1->stmtList) {
                extractPatternHelper(stmtNode, resultVec, stmtNumbers);
            }
            return;
        }
        switch (nodeType) {
            case AST::ASSIGN_NODE: {
                shared_ptr<AssignNode> assignNode = dynamic_pointer_cast<AssignNode>(node);
                int lineNum = stmtNumbers->at(assignNode);
                resultVec->insert(resultVec->begin(), pair(pair(lineNum, assignNode->variableNode->variable), assignNode));
                return;
            }
            case AST::IF_NODE: {
                shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(node);
                for (auto stmtNode: ifNode->ifStmtList) {
                    extractPatternHelper(stmtNode, resultVec, stmtNumbers);
                }
                for(auto stmtNode: ifNode->elseStmtList) {
                    extractPatternHelper(stmtNode, resultVec, stmtNumbers);
                }
                return;
            }
            case AST::WHILE_NODE: {
                shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(node);
                for(auto stmtNode: whileNode->stmtList) {
                    extractPatternHelper(stmtNode, resultVec, stmtNumbers);
                }
                return;
            }
        }

    }
} // DE