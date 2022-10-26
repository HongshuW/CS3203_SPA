//
// Created by Xingchen Lin on 26/10/22.
//

#include "DesignExtractorUtils.h"

namespace DE {
DesignExtractorUtils::DesignExtractorUtils() = default;

unordered_map<string, vector<shared_ptr<CallNode>>>
DesignExtractorUtils::extractCallNodesFromProcedures(
    const shared_ptr<AST::ProgramNode>& rootPtr) {
  auto mapCallNodesToProcedures =
      unordered_map<string, vector<shared_ptr<CallNode>>>();
  vector<shared_ptr<ProcedureNode>> procedureList = rootPtr->procedureList;
  for (const auto& procedureNode : procedureList) {
    string name = procedureNode->procedureName;
    auto listOfCallNodes = vector<shared_ptr<CallNode>>();
    queue<vector<shared_ptr<StmtNode>>> queue;
    queue.push(procedureNode->stmtList);
    while (!queue.empty()) {
      auto stmtList = queue.front();
      queue.pop();
      for (const auto& stmtNode : stmtList) {
        NodeType nodeType = ASTUtils::getNodeType(stmtNode);
        switch (nodeType) {
          case AST::CALL_NODE: {
            shared_ptr<CallNode> callNode =
                dynamic_pointer_cast<CallNode>(stmtNode);
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
            shared_ptr<WhileNode> whileNode =
                dynamic_pointer_cast<WhileNode>(stmtNode);
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

void DesignExtractorUtils::extractVariablesFromCallNodesInProceduresToList(
    const shared_ptr<AST::CallNode>& callNode,
    unordered_map<std::string, unordered_set<std::string>>
        mappedProceduresToVar,
    unordered_map<std::string, vector<shared_ptr<AST::CallNode>>>
        mappedCallNodesToProcedures,
    unordered_set<std::string>& uniqueVarList) {
  queue<shared_ptr<CallNode>> queue;
  queue.push(callNode);
  while (!queue.empty()) {
    auto callNodeEntry = queue.front();
    queue.pop();
    unordered_set<string> usedVarList = unordered_set<string>();
    if (mappedProceduresToVar.count(callNodeEntry->procedureName)) {
      usedVarList = mappedProceduresToVar.at(callNodeEntry->procedureName);
    }

    uniqueVarList.insert(usedVarList.begin(), usedVarList.end());

    if (mappedCallNodesToProcedures.count(callNodeEntry->procedureName) != 0) {
      auto otherCallNodes =
          mappedCallNodesToProcedures.at(callNodeEntry->procedureName);
      for (const auto& n : otherCallNodes) {
        queue.push(n);
      }
    }
  }
}

vector<shared_ptr<StmtNode>>
DesignExtractorUtils::extractIfAndWhileNodesFromProcedures(
    const shared_ptr<AST::ProgramNode>& rootPtr) {
  vector<shared_ptr<StmtNode>> extractedNodes;
  for (const auto& procedureNode : rootPtr->procedureList) {
    queue<vector<shared_ptr<StmtNode>>> queue;
    queue.push(procedureNode->stmtList);
    while (!queue.empty()) {
      auto stmtList = queue.front();
      queue.pop();
      for (const auto& stmtNode : stmtList) {
        NodeType nodeType = ASTUtils::getNodeType(stmtNode);
        switch (nodeType) {
          case AST::IF_NODE: {
            shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(stmtNode);
            extractedNodes.push_back(ifNode);
            queue.push(ifNode->ifStmtList);
            queue.push(ifNode->elseStmtList);
            break;
          }
          case AST::WHILE_NODE: {
            shared_ptr<WhileNode> whileNode =
                dynamic_pointer_cast<WhileNode>(stmtNode);
            extractedNodes.push_back(whileNode);
            queue.push(whileNode->stmtList);
            break;
          }
          default:
            break;
        }
      }
    }
  }

  return extractedNodes;
}

void DesignExtractorUtils::extractCallStmtRelationshipsToOutput(
    int stmtNo, const shared_ptr<CallNode>& callNode,
    unordered_map<string, unordered_set<string>> mappedProceduresToVars,
    unordered_map<string, vector<shared_ptr<CallNode>>>
        mappedCallNodesToProcedures,
    const shared_ptr<list<vector<string>>>& output) {
  queue<shared_ptr<CallNode>> queue;
  queue.push(callNode);
  while (!queue.empty()) {
    auto callNodeEntry = queue.front();
    queue.pop();
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
        queue.push(n);
      }
    }
  }
}

void DesignExtractorUtils::extractCallStmtRelationshipsWithIfAndWhileToOutput(
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

    while (!queue.empty()) {
      auto nodeEntry = queue.front();
      queue.pop();
      NodeType nodeType = ASTUtils::getNodeType(nodeEntry);
      switch (nodeType) {
        case AST::CALL_NODE: {
          shared_ptr<CallNode> callNode =
              dynamic_pointer_cast<CallNode>(nodeEntry);
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
}  // namespace DE