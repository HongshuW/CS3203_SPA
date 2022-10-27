//
// Created by Nafour on 27/10/22.
//

#include "UsesSRelationExtractor.h"

#include "AST/utils/ASTUtils.h"
#include "design_extractor/results/ExtractorResult.h"
#include "design_extractor/results/RelationResult.h"
namespace DE {
UsesSRelationExtractor::UsesSRelationExtractor(
    shared_ptr<DataModifier> dataModifier, shared_ptr<ProgramNode> programNode)
    : UsesRelationExtractor(dataModifier, programNode) {
  ifWhileStmtNoToUsedVarsMap =
      make_shared<unordered_map<string, shared_ptr<unordered_set<string>>>>();
  initIfAndWhileStmtNoToUsedVarsMap();
}
shared_ptr<ExtractorResult> UsesSRelationExtractor::extract() {
  for (auto procedureNode : programNode->procedureList) {
    extractUsesSHelper(procedureNode, output);
    // save Uses relation from each procedure in result
    auto it = output->begin();
    for (auto pair : *output) {
      it = output->insert(it, pair);
    }
  }
  insertCallsForUseS();
  output->sort();
  output->unique();
  return make_shared<RelationResult>(output);
}

void UsesSRelationExtractor::save(shared_ptr<ExtractorResult> result) {
  shared_ptr<RelationResult> usesSResult =
      static_pointer_cast<RelationResult>(result);
  for (auto entry : *usesSResult->getResult()) {
    dataModifier->saveUsesS(entry);
  }
}

unordered_set<string> UsesSRelationExtractor::extractUsesSHelper(
    shared_ptr<ASTNode> node, shared_ptr<list<vector<string>>> result) {
  NodeType nodeType = ASTUtils::getNodeType(node);

  if (nodeType == AST::PROCEDURE_NODE) {
    shared_ptr<ProcedureNode> procedureNode1 =
        dynamic_pointer_cast<ProcedureNode>(node);
    unordered_set<string> usedVarsProcedure = unordered_set<string>();
    for (auto stmtNode : procedureNode1->stmtList) {
      auto fromChild = extractUsesSHelper(stmtNode, result);
      usedVarsProcedure.insert(fromChild.begin(), fromChild.end());
    }
    return usedVarsProcedure;
  }

  switch (nodeType) {
    case AST::PRINT_NODE: {
      shared_ptr<PrintNode> printNode = dynamic_pointer_cast<PrintNode>(node);
      int stmtNum = stmtNumbers->at(printNode);
      string var = printNode->variableNode->variable;
      result->insert(result->begin(), {to_string(stmtNum), var});
      return unordered_set<string>({var});
    }
    case AST::IF_NODE: {
      shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(node);
      unordered_set<string> usedVarsIf = unordered_set<string>();

      for (auto child : ifNode->ifStmtList) {
        auto fromChild = extractUsesSHelper(child, result);
        usedVarsIf.insert(fromChild.begin(), fromChild.end());
      }

      for (auto child : ifNode->elseStmtList) {
        auto fromChild = extractUsesSHelper(child, result);
        usedVarsIf.insert(fromChild.begin(), fromChild.end());
      }

      auto variables = getVariablesFromCondExprNode(ifNode->condExpr);
      usedVarsIf.insert(variables.begin(), variables.end());

      for (const auto& usedVarIf : usedVarsIf) {
        int stmtNum = stmtNumbers->at(ifNode);
        result->insert(result->begin(), {to_string(stmtNum), usedVarIf});
      }
      return usedVarsIf;
    }
    case AST::WHILE_NODE: {
      shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(node);
      unordered_set<string> usedVarsWhile = unordered_set<string>();

      for (auto child : whileNode->stmtList) {
        auto fromChild = extractUsesSHelper(child, result);
        usedVarsWhile.insert(fromChild.begin(), fromChild.end());
      }

      auto variables = getVariablesFromCondExprNode(whileNode->condExpr);
      usedVarsWhile.insert(variables.begin(), variables.end());

      for (const auto& usedVarWhile : usedVarsWhile) {
        int stmtNum = stmtNumbers->at(whileNode);
        result->insert(result->begin(), {to_string(stmtNum), usedVarWhile});
      }
      return usedVarsWhile;
    }
    case AST::ASSIGN_NODE: {
      shared_ptr<AssignNode> assignNode =
          dynamic_pointer_cast<AssignNode>(node);
      auto variables = getVariablesFromExprNode(assignNode->expressionNode);
      for (const auto& var : variables) {
        int stmtNum = stmtNumbers->at(assignNode);
        result->insert(result->begin(), {to_string(stmtNum), var});
      }
      return variables;
    }
    default: {
      return unordered_set<string>();
    }
  }
}

void UsesSRelationExtractor::insertCallsForUseS() {
  auto mappedCallNodesToProcedures =
      extractCallNodesFromProcedures(programNode);
  for (auto pair : mappedCallNodesToProcedures) {
    vector<shared_ptr<CallNode>> listOfCallNodes = pair.second;
    for (auto callNode : listOfCallNodes) {
      int stmtNo = stmtNumbers->at(callNode);
      extractCallStmtRelationshipsToOutput(stmtNo, callNode,
                                           proceduresToUsedVarsMap,
                                           mappedCallNodesToProcedures, output);
    }
  }
  insertCallsInIfAndWhileForUseS();
}

void UsesSRelationExtractor::insertCallsInIfAndWhileForUseS() {
  extractCallStmtRelationshipsWithIfAndWhileToOutput(
      programNode, proceduresToUsedVarsMap, ifWhileStmtNoToUsedVarsMap, output);
}

void UsesSRelationExtractor::initIfAndWhileStmtNoToUsedVarsMap() {
  auto ifAndWhileNodeList = extractIfAndWhileNodesFromProcedures(programNode);
  for (auto node : ifAndWhileNodeList) {
    auto uniqueVarList = make_shared<unordered_set<string>>();
    int stmtNo = stmtNumbers->at(node);
    queue<shared_ptr<StmtNode>> queue;
    queue.push(node);
    while (!queue.empty()) {
      auto nodeEntry = queue.front();
      queue.pop();
      NodeType nodeType = ASTUtils::getNodeType(nodeEntry);
      switch (nodeType) {
        case AST::IF_NODE: {
          shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(nodeEntry);
          auto variables = getVariablesFromCondExprNode(ifNode->condExpr);
          uniqueVarList->insert(variables.begin(), variables.end());
          for (auto n : ifNode->ifStmtList) {
            queue.push(n);
          }
          for (auto n : ifNode->elseStmtList) {
            queue.push(n);
          }
          break;
        }
        case AST::WHILE_NODE: {
          shared_ptr<WhileNode> whileNode =
              dynamic_pointer_cast<WhileNode>(nodeEntry);
          auto variables = getVariablesFromCondExprNode(whileNode->condExpr);
          uniqueVarList->insert(variables.begin(), variables.end());
          for (auto n : whileNode->stmtList) {
            queue.push(n);
          }
          break;
        }

        case AST::ASSIGN_NODE: {
          shared_ptr<AssignNode> assignNode =
              dynamic_pointer_cast<AssignNode>(nodeEntry);
          auto variables = getVariablesFromExprNode(assignNode->expressionNode);
          uniqueVarList->insert(variables.begin(), variables.end());
          break;
        }

        case AST::PRINT_NODE: {
          shared_ptr<PrintNode> printNode =
              dynamic_pointer_cast<PrintNode>(nodeEntry);
          string var = printNode->variableNode->variable;
          uniqueVarList->insert(var);
          break;
        }
        default:
          break;
      }
    }
    ifWhileStmtNoToUsedVarsMap->insert(
        make_pair(to_string(stmtNo), uniqueVarList));
  }
}
}  // namespace DE