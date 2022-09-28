//
// Created by Nafour on 10/9/22.
//

#include "EntityExtractor.h"
#include <queue>
#include "utils/Utils.h"

namespace DE {
    unordered_set<string>
    EntityExtractor::extractDesignEntity(shared_ptr<StmtNode> node, QB::DesignEntity designEntityType) {
        unordered_set<string> ans = unordered_set<string>();
        switch (designEntityType) {
            case QB::DesignEntity::VARIABLE: {
                return *extractVariables(node);
            }
            case QB::DesignEntity::CONSTANT: {
                return *extractConstants(node);
            }
            default:{
                break;
            }
        }
        return ans;
    }
    shared_ptr<unordered_set<string>> EntityExtractor::extractVariables(shared_ptr<StmtNode> stmtNode) {
        shared_ptr<unordered_set<string>> set = make_shared<unordered_set<string>>();

        NodeType nodeType = ASTUtils::getNodeType(stmtNode);
        switch (nodeType) {
            case AST::PRINT_NODE: {
                shared_ptr<PrintNode> printNode = dynamic_pointer_cast<PrintNode>(stmtNode);
                set->insert(printNode->variableNode->variable);
                break;
            }
            case AST::ASSIGN_NODE: {
                shared_ptr<AssignNode> assignNode = dynamic_pointer_cast<AssignNode>(stmtNode);
                set->insert(assignNode->variableNode->variable);
                unordered_set<string> variables = getVariablesFromExprNode(assignNode->expressionNode);
                set->insert(variables.begin(), variables.end());
                break;
            }
            case AST::IF_NODE: {
                shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(stmtNode);
                unordered_set<string> variables = getVariablesFromExprString(ifNode->condExpr->condExpr);
                set->insert(variables.begin(), variables.end());
                break;
            }
            case AST::READ_NODE:{
                shared_ptr<ReadNode> readNode = dynamic_pointer_cast<ReadNode>(stmtNode);
                set->insert(readNode->variableNode->variable);
                break;
            }
            case AST::WHILE_NODE: {
                shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(stmtNode);
                unordered_set<string> variables = getVariablesFromExprString(whileNode->condExpr->condExpr);
                set->insert(variables.begin(), variables.end());
                break;
            }
            default: {
                break;
            }
        }
        return set;

    }

    shared_ptr<unordered_set<string>> EntityExtractor::extractConstants(shared_ptr<StmtNode> stmtNode) {
        shared_ptr<unordered_set<string>> set = make_shared<unordered_set<string>>();

        NodeType nodeType = ASTUtils::getNodeType(stmtNode);
        switch (nodeType) {
            case AST::ASSIGN_NODE: {
                shared_ptr<AssignNode> assignNode = dynamic_pointer_cast<AssignNode>(stmtNode);
                unordered_set<string> constants = getConstantsFromExprNode(assignNode->expressionNode);
                set->insert(constants.begin(), constants.end());
                break;
            }
            case AST::IF_NODE: {
                shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(stmtNode);
                unordered_set<string> constants = getConstantsFromExprString(ifNode->condExpr->condExpr);
                set->insert(constants.begin(), constants.end());
                break;
            }
            case AST::WHILE_NODE: {
                shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(stmtNode);
                unordered_set<string> constants = getConstantsFromExprString(whileNode->condExpr->condExpr);
                set->insert(constants.begin(), constants.end());
                break;
            }
            default: {
                break;
            }
        }
        return set;

    }

    unordered_set<string> EntityExtractor::getVariablesFromExprString(string expr) {
        unordered_set<string> ans;
        unordered_set<char> ignored = {'*', '/', '%', '+', '-', '(', ')', ' ', '=', '<', '>', '!', '|', '&'};
        string acc = "";
        for (int i = 0 ; i < expr.length(); i++) {
            if (ignored.find(expr[i]) == ignored.end()) { // char not in ignored
                acc += expr[i];
            } else {
                if (!acc.empty() && !is_number(acc)) {
                    ans.insert(acc);
                }
                acc = "";
            }
        }
        if (!acc.empty() && !is_number(acc)) {
            ans.insert(acc);
        }
        return ans;
    }

    unordered_set<string> EntityExtractor::getConstantsFromExprString(string expr) {
        unordered_set<string> ans;
        unordered_set<char> ignored = {'*', '/', '%', '+', '-', '(', ')', ' ', '=', '<', '>', '!', '|', '&'};
        string acc = "";
        for (int i = 0 ; i < expr.length(); i++) {
            if (ignored.find(expr[i]) == ignored.end()) { // char not in ignored
                acc += expr[i];
            } else {
                if (!acc.empty() && is_number(acc)) {
                    ans.insert(acc);
                }
                acc = "";
            }
        }
        if (!acc.empty() && is_number(acc)) {
            ans.insert(acc);
        }
        return ans;
    }

    bool EntityExtractor::is_number(const std::string& s)
    {
        std::string::const_iterator it = s.begin();
        while (it != s.end() && std::isdigit(*it)) ++it;
        return !s.empty() && it == s.end();
    }

    unordered_set<string> EntityExtractor::getVariablesFromExprNode(shared_ptr<ExprNode> exprNode) {
        unordered_set<string> ans;
        queue<shared_ptr<ExprNode>> queue;
        queue.push(exprNode);

        while (!queue.empty()) {
            auto currNode = queue.front();
            queue.pop();
            if (Utils::isValidName(currNode->expr)) {
                ans.insert(currNode->expr);
            }
            if (currNode->left) {
                queue.push(currNode->left);
            }
            if (currNode->right) {
                queue.push(currNode->right);
            }
        }
        return ans;
    }

    unordered_set<string> EntityExtractor::getConstantsFromExprNode(shared_ptr<ExprNode> exprNode) {
        unordered_set<string> ans;
        queue<shared_ptr<ExprNode>> queue;
        queue.push(exprNode);

        while (!queue.empty()) {
            auto currNode = queue.front();
            queue.pop();
            if (Utils::isValidNumber(currNode->expr)) {
                ans.insert(currNode->expr);
            }
            if (currNode->left) {
                queue.push(currNode->left);
            }
            if (currNode->right) {
                queue.push(currNode->right);
            }
        }
        return ans;
    }

    unordered_map<string, vector<shared_ptr<CallNode>>> EntityExtractor::extractCallNodesFromProcedures(shared_ptr<AST::ProgramNode> rootPtr) {
        auto mapCallNodesToProcedures = unordered_map<string, vector<shared_ptr<CallNode>>>();
        vector<shared_ptr<ProcedureNode>> procedureList = rootPtr->procedureList;
        for (auto procedureNode : procedureList) {
            string name = procedureNode->procedureName;
            auto listOfCallNodes = vector<shared_ptr<CallNode>>();
            queue<vector<shared_ptr<StmtNode>>> queue;
            queue.push(procedureNode->stmtList);
            while (!queue.empty()) {
                auto stmtList = queue.front();
                queue.pop();
                for (auto stmtNode : stmtList) {
                    NodeType nodeType = ASTUtils::getNodeType(stmtNode);
                    switch (nodeType) {
                    case AST::CALL_NODE: {
                        shared_ptr<CallNode> callNode = dynamic_pointer_cast<CallNode>(stmtNode);
                        listOfCallNodes.push_back(callNode);
                        break;
                    }
                    case AST::IF_NODE: {
                        shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(stmtNode);
                        vector<shared_ptr<StmtNode>> ifStmtList = ifNode->ifStmtList;
                        vector<shared_ptr<StmtNode>> elseStmtList = ifNode->elseStmtList;
                        queue.push(ifStmtList);
                        queue.push(elseStmtList);
                        break;
                    }
                    case AST::WHILE_NODE: {
                        shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(stmtNode);
                        vector<shared_ptr<StmtNode>> whileStmtList = whileNode->stmtList;
                        queue.push(whileStmtList);
                        break;
                    }
                    default:
                        break;
                    }
                }
            }
            if (!listOfCallNodes.empty()) {
                mapCallNodesToProcedures.insert(make_pair(name, listOfCallNodes));
            }
        }
        return mapCallNodesToProcedures;
    }
} // DE