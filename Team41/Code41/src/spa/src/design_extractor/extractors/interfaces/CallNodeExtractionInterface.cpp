//
// Created by Nafour on 27/10/22.
//

#include "CallNodeExtractionInterface.h"

namespace DE {
unordered_map<string, vector<shared_ptr<CallNode>>>
CallNodeExtractionInterface::extractCallNodesFromProcedures(
    const shared_ptr<ProgramNode>& rootPtr) {
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

void CallNodeExtractionInterface::extractCallStmtRelationshipsToOutput(
    int stmtNo, const shared_ptr<CallNode>& callNode,
    StrToSetMap mappedProceduresToVars,
    unordered_map<string, vector<shared_ptr<CallNode>>>
        mappedCallNodesToProcedures,
    const shared_ptr<list<vector<string>>>& output) {
  queue<shared_ptr<CallNode>> queue;
  queue.push(callNode);
  while (!queue.empty()) {
    auto callNodeEntry = queue.front();
    queue.pop();
    shared_ptr<unordered_set<string>> varList =
        make_shared<unordered_set<string>>();
    if (mappedProceduresToVars->count(callNodeEntry->procedureName) != 0) {
      varList = mappedProceduresToVars->at(callNodeEntry->procedureName);
    }
    for (const auto& var : *varList) {
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

void CallNodeExtractionInterface::
    extractCallStmtRelationshipsWithIfAndWhileToOutput(
        const shared_ptr<ProgramNode>& rootPtr,
        StrToSetMap mappedProceduresToVars, StrToSetMap mappedIfAndWhileToVars,
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
          shared_ptr<unordered_set<string>> varList =
              make_shared<unordered_set<string>>();
          if (mappedProceduresToVars->count(callNode->procedureName) != 0) {
            varList = mappedProceduresToVars->at(callNode->procedureName);
          }
          shared_ptr<unordered_set<string>> usedVarList =
              make_shared<unordered_set<string>>();
          if (mappedIfAndWhileToVars->count(to_string(stmtNo)) != 0) {
            usedVarList = mappedIfAndWhileToVars->at(to_string(stmtNo));
          }
          for (const auto& var : *varList) {
            if (usedVarList->count(var) == 0) {
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

vector<shared_ptr<StmtNode>>
CallNodeExtractionInterface::extractIfAndWhileNodesFromProcedures(
    const shared_ptr<ProgramNode>& rootPtr) {
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
}  // namespace DE