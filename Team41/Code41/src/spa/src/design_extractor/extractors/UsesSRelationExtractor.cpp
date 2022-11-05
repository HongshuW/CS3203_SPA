//
// Created by Nafour on 27/10/22.
//
#include "UsesSRelationExtractor.h"

#include <queue>
#include <utility>

#include "AST/utils/ASTUtils.h"
#include "design_extractor/results/ExtractorResult.h"
#include "design_extractor/results/RelationResult.h"

namespace DE {
UsesSRelationExtractor::UsesSRelationExtractor(
    shared_ptr<DataModifier> dataModifier, shared_ptr<ProgramNode> programNode)
    : UsesRelationExtractor(std::move(dataModifier), std::move(programNode)) {}
shared_ptr<ExtractorResult> UsesSRelationExtractor::extract() {
  shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
      ASTUtils::getNodePtrToLineNumMap(programNode);
  shared_ptr<vector<vector<vector<string>>>> resultOfProcedures =
      make_shared<vector<vector<vector<string>>>>();
  shared_ptr<list<vector<string>>> ans = make_shared<list<vector<string>>>();

  for (const auto& procedureNode : programNode->procedureList) {
    shared_ptr<vector<vector<string>>> result =
        make_shared<vector<vector<string>>>();
    extractUsesSHelper(procedureNode, result, stmtNumbers);

    // save Uses relation from each procedure in result
    auto it = ans->begin();
    for (const auto& pair : *result) {
      it = ans->insert(it, pair);
    }
  }
  insertCallsForUseS(programNode, ans);
  return make_shared<RelationResult>(ans);
}

void UsesSRelationExtractor::save(shared_ptr<ExtractorResult> result) {
  shared_ptr<RelationResult> usesSResult =
      static_pointer_cast<RelationResult>(result);
  for (const auto& entry : *usesSResult->getResult()) {
    dataModifier->saveUsesS(entry);
  }
}

unordered_set<string> UsesSRelationExtractor::extractUsesSHelper(
    const shared_ptr<ASTNode>& node, const shared_ptr<vector<vector<string>>>& result,
    const shared_ptr<unordered_map<shared_ptr<StmtNode>, int>>& stmtNumbers) {
  NodeType nodeType = ASTUtils::getNodeType(node);

  if (nodeType == AST::PROCEDURE_NODE) {
    shared_ptr<ProcedureNode> procedureNode1 =
        dynamic_pointer_cast<ProcedureNode>(node);
    unordered_set<string> usedVarsProcedure = unordered_set<string>();
    for (const auto& stmtNode : procedureNode1->stmtList) {
      auto fromChild = extractUsesSHelper(stmtNode, result, stmtNumbers);
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

      for (const auto& child : ifNode->ifStmtList) {
        auto fromChild = extractUsesSHelper(child, result, stmtNumbers);
        usedVarsIf.insert(fromChild.begin(), fromChild.end());
      }

      for (const auto& child : ifNode->elseStmtList) {
        auto fromChild = extractUsesSHelper(child, result, stmtNumbers);
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

      for (const auto& child : whileNode->stmtList) {
        auto fromChild = extractUsesSHelper(child, result, stmtNumbers);
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
      return {};
    }
  }
}

unordered_map<string, unordered_set<string>>
UsesSRelationExtractor::mapProceduresToUsedVariables(
    const shared_ptr<ProgramNode>& rootPtr) {
  shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
      ASTUtils::getNodePtrToLineNumMap(rootPtr);
  shared_ptr<vector<vector<vector<string>>>> resultOfProcedures =
      make_shared<vector<vector<vector<string>>>>();
  auto map = unordered_map<string, unordered_set<string>>();

  for (const auto& procedureNode : rootPtr->procedureList) {
    shared_ptr<vector<vector<string>>> result =
        make_shared<vector<vector<string>>>();
    extractUsesSHelper(procedureNode, result, stmtNumbers);
    string procedureName = procedureNode->procedureName;
    unordered_set<string> variableList;
    for (auto pair : *result) {
      variableList.insert(pair[1]);
    }

    map.insert(make_pair(procedureName, variableList));
  }

  return map;
}

unordered_map<string, unordered_set<string>>
UsesSRelationExtractor::mapIfAndWhileStmtNoToUsedVariables(
    const shared_ptr<ProgramNode>& rootPtr) {
  shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
      ASTUtils::getNodePtrToLineNumMap(rootPtr);
  auto map = unordered_map<string, unordered_set<string>>();

  auto ifAndWhileNodeList = extractIfAndWhileNodesFromProcedures(rootPtr);

  for (const auto& node : ifAndWhileNodeList) {
    auto uniqueVarList = unordered_set<string>();
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
          uniqueVarList.insert(variables.begin(), variables.end());
          for (const auto& n : ifNode->ifStmtList) {
            queue.push(n);
          }
          for (const auto& n : ifNode->elseStmtList) {
            queue.push(n);
          }
          break;
        }
        case AST::WHILE_NODE: {
          shared_ptr<WhileNode> whileNode =
              dynamic_pointer_cast<WhileNode>(nodeEntry);
          auto variables = getVariablesFromCondExprNode(whileNode->condExpr);
          uniqueVarList.insert(variables.begin(), variables.end());
          for (const auto& n : whileNode->stmtList) {
            queue.push(n);
          }
          break;
        }

        case AST::ASSIGN_NODE: {
          shared_ptr<AssignNode> assignNode =
              dynamic_pointer_cast<AssignNode>(nodeEntry);
          auto variables = getVariablesFromExprNode(assignNode->expressionNode);
          uniqueVarList.insert(variables.begin(), variables.end());
          break;
        }

        case AST::PRINT_NODE: {
          shared_ptr<PrintNode> printNode =
              dynamic_pointer_cast<PrintNode>(nodeEntry);
          string var = printNode->variableNode->variable;
          uniqueVarList.insert(var);
          break;
        }
        default:
          break;
      }
    }
    map.insert(make_pair(to_string(stmtNo), uniqueVarList));
  }

  return map;
}

void UsesSRelationExtractor::extractCallStmtRelationshipsWithIfAndWhileToOutput(
    const shared_ptr<ProgramNode>& rootPtr,
    unordered_map<string, unordered_set<string>> mappedProceduresToVars,
    unordered_map<string, unordered_set<string>> mappedIfAndWhileToVars,
    const shared_ptr<list<vector<string>>>& output) {
  auto ifAndWhileNodeList = extractIfAndWhileNodesFromProcedures(rootPtr);
  shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
      ASTUtils::getNodePtrToLineNumMap(rootPtr);
  auto mappedCallNodesToProcedures = extractCallNodesFromProcedures(rootPtr);

  for (const auto& node : ifAndWhileNodeList) {
    auto uniqueVarList = unordered_set<string>();
    int stmtNo = stmtNumbers->at(node);
    queue<shared_ptr<StmtNode>> queue;
    queue.push(node);
    unordered_set<string> visitedProcs;
    while (!queue.empty()) {
      auto nodeEntry = queue.front();
      queue.pop();
      NodeType nodeType = ASTUtils::getNodeType(nodeEntry);
      switch (nodeType) {
        case AST::CALL_NODE: {
          shared_ptr<CallNode> callNode =
              dynamic_pointer_cast<CallNode>(nodeEntry);
          if (visitedProcs.count(callNode->procedureName)) continue;
          visitedProcs.insert(callNode->procedureName);
          unordered_set<string> varList;
          if (mappedProceduresToVars.count(callNode->procedureName) != 0) {
            varList = mappedProceduresToVars.at(callNode->procedureName);
          }
          unordered_set<string> usedVarList;
          if (mappedIfAndWhileToVars.count(to_string(stmtNo)) != 0) {
            usedVarList = mappedIfAndWhileToVars.at(to_string(stmtNo));
          }
          for (const auto& var : varList) {
            if (usedVarList.count(var) == 0) {
              uniqueVarList.insert(var);
            }
          }

          if (mappedCallNodesToProcedures.count(callNode->procedureName) != 0) {
            auto otherCallNodes =
                mappedCallNodesToProcedures.at(callNode->procedureName);
            for (const auto& n : otherCallNodes) {
              if (visitedProcs.count(n->procedureName)) continue;
              queue.push(n);
            }
          }
          break;
        }

        case AST::IF_NODE: {
          shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(nodeEntry);
          for (const auto& n : ifNode->ifStmtList) {
            queue.push(n);
          }
          for (const auto& n : ifNode->elseStmtList) {
            queue.push(n);
          }
          break;
        }
        case AST::WHILE_NODE: {
          shared_ptr<WhileNode> whileNode =
              dynamic_pointer_cast<WhileNode>(nodeEntry);
          for (const auto& n : whileNode->stmtList) {
            queue.push(n);
          }
          break;
        }

        default:
          break;
      }
    }

    for (const auto& var : uniqueVarList) {
      vector<string> callEntry;
      callEntry.push_back(to_string(stmtNo));
      callEntry.push_back(var);
      output->push_back(callEntry);
    }
  }
}

void UsesSRelationExtractor::insertCallsInIfAndWhileForUseS(
    const shared_ptr<ProgramNode>& rootPtr, const shared_ptr<list<vector<string>>>& ans) {
  auto mappedProceduresToUsedVar = mapProceduresToUsedVariables(rootPtr);
  auto mappedIfAndWhileStmtNoToUsedVariables =
      mapIfAndWhileStmtNoToUsedVariables(rootPtr);
  extractCallStmtRelationshipsWithIfAndWhileToOutput(
      rootPtr, mappedProceduresToUsedVar, mappedIfAndWhileStmtNoToUsedVariables,
      ans);
}

void UsesSRelationExtractor::extractCallStmtRelationshipsToOutput(
    int stmtNo, const shared_ptr<CallNode>& callNode,
    unordered_map<string, unordered_set<string>> mappedProceduresToVars,
    unordered_map<string, vector<shared_ptr<CallNode>>>
        mappedCallNodesToProcedures,
    const shared_ptr<list<vector<string>>>& output) {
  queue<shared_ptr<CallNode>> queue;
  queue.push(callNode);
  unordered_set<string> visitedProcs;
  while (!queue.empty()) {
    auto callNodeEntry = queue.front();
    queue.pop();
    if (visitedProcs.count(callNodeEntry->procedureName)) continue;
    visitedProcs.insert(callNodeEntry->procedureName);
    unordered_set<string> varList;
    if (mappedProceduresToVars.count(callNodeEntry->procedureName) != 0) {
      varList = mappedProceduresToVars.at(callNodeEntry->procedureName);
    }
    for (const auto& var : varList) {
      vector<string> relationshipCallEntry;
      relationshipCallEntry.push_back(to_string(stmtNo));
      relationshipCallEntry.push_back(var);
      output->push_back(relationshipCallEntry);
    }

    if (mappedCallNodesToProcedures.count(callNodeEntry->procedureName) != 0) {
      auto otherCallNodes =
          mappedCallNodesToProcedures.at(callNodeEntry->procedureName);
      for (const auto& n : otherCallNodes) {
        if (visitedProcs.count(n->procedureName)) continue;
        queue.push(n);
      }
    }
  }
}

void UsesSRelationExtractor::insertCallsForUseS(
    const shared_ptr<ProgramNode>& rootPtr, shared_ptr<list<vector<string>>> ans) {
  auto mappedProceduresToUsedVar = mapProceduresToUsedVariables(rootPtr);
  auto mappedCallNodesToProcedures = extractCallNodesFromProcedures(rootPtr);
  shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
      ASTUtils::getNodePtrToLineNumMap(rootPtr);

  for (const auto& pair : mappedCallNodesToProcedures) {
    vector<shared_ptr<CallNode>> listOfCallNodes = pair.second;
    for (const auto& callNode : listOfCallNodes) {
      int stmtNo = stmtNumbers->at(callNode);
      extractCallStmtRelationshipsToOutput(stmtNo, callNode,
                                           mappedProceduresToUsedVar,
                                           mappedCallNodesToProcedures, ans);
    }
  }

  insertCallsInIfAndWhileForUseS(rootPtr, ans);
}
}  // namespace DE