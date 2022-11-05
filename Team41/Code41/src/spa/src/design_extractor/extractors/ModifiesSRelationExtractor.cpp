//
// Created by Nafour on 27/10/22.
//

#include "ModifiesSRelationExtractor.h"

#include <queue>

#include "AST/utils/ASTUtils.h"
#include "design_extractor/results/RelationResult.h"

namespace DE {
ModifiesSRelationExtractor::ModifiesSRelationExtractor(
    shared_ptr<DataModifier> dataModifier, shared_ptr<ProgramNode> programNode)
    : ModifiesRelationExtractor(dataModifier, programNode) {
  ancestors = make_shared<vector<string>>();
  output = make_shared<list<vector<string>>>();
  ifWhileStmtNoToModifiedVarsMap =
      make_shared<unordered_map<string, shared_ptr<unordered_set<string>>>>();
  initIfAndWhileStmtNoToModifiedVarsMap();
}

shared_ptr<ExtractorResult> ModifiesSRelationExtractor::extract() {
  queue<shared_ptr<ASTNode>> queue;
  queue.push(programNode);
  while (!queue.empty()) {
    shared_ptr<ASTNode> current = queue.front();
    queue.pop();
    NodeType nodeType = ASTUtils::getNodeType(current);
    if (nodeType == NodeType::ASSIGN_NODE) {
      // encounter an assign node, add modifiesS relation
      shared_ptr<AssignNode> ptr = dynamic_pointer_cast<AssignNode>(current);
      int stmtNo = stmtNumbers->at(ptr);
      shared_ptr<VariableNode> variableNode = ptr->variableNode;
      vector<string> row{to_string(stmtNo), variableNode->variable};
      output->push_back(row);
    } else if (nodeType == NodeType::READ_NODE) {
      // encounter a read node, add modifiesS relation
      shared_ptr<ReadNode> ptr = dynamic_pointer_cast<ReadNode>(current);
      int stmtNo = stmtNumbers->at(ptr);
      shared_ptr<VariableNode> variableNode = ptr->variableNode;
      vector<string> row{to_string(stmtNo), variableNode->variable};
      output->push_back(row);
    } else if (nodeType == NodeType::WHILE_NODE) {
      // encounter a while node, check stmtList
      shared_ptr<WhileNode> ptr = dynamic_pointer_cast<WhileNode>(current);
      extractorHelper(ptr);
    } else if (nodeType == NodeType::IF_NODE) {
      // encounter an if node, check ifStmts and elseStmts
      shared_ptr<IfNode> ptr = dynamic_pointer_cast<IfNode>(current);
      extractorHelper(ptr);
    } else if (nodeType == NodeType::PROCEDURE_NODE) {
      // encounter a procedure node, check its statements
      shared_ptr<ProcedureNode> ptr =
          dynamic_pointer_cast<ProcedureNode>(current);
      vector<shared_ptr<StmtNode>> stmtList = ptr->stmtList;
      for (shared_ptr<StmtNode> n : stmtList) {
        queue.push(n);
      }
    } else if (nodeType == NodeType::PROGRAM_NODE) {
      // encounter a program node, check its procedures
      shared_ptr<ProgramNode> ptr = dynamic_pointer_cast<ProgramNode>(current);
      vector<shared_ptr<ProcedureNode>> procedureList = ptr->procedureList;
      for (shared_ptr<ProcedureNode> n : procedureList) {
        queue.push(n);
      }
    }
  }

  insertCallsForModifiesS();

  output->sort();
  output->unique();
  return make_shared<RelationResult>(output);
}

void ModifiesSRelationExtractor::extractorHelper(shared_ptr<ASTNode> node) {
  NodeType nodeType = ASTUtils::getNodeType(node);

  switch (nodeType) {
    case AST::ASSIGN_NODE: {
      // add all {ancestor, variable} pairs to output
      shared_ptr<AssignNode> ptr = dynamic_pointer_cast<AssignNode>(node);
      int stmtNo = stmtNumbers->at(ptr);
      ancestors->push_back(to_string(stmtNo));
      for (string ancestor : *ancestors) {
        vector<string> row{ancestor, ptr->variableNode->variable};
        output->push_back(row);
      }
      // remove current node from ancestors list
      ancestors->pop_back();
      break;
    }
    case AST::READ_NODE: {
      // add all {ancestor, variable} pairs to output
      shared_ptr<ReadNode> ptr = dynamic_pointer_cast<ReadNode>(node);
      int stmtNo = stmtNumbers->at(ptr);
      ancestors->push_back(to_string(stmtNo));
      for (string ancestor : *ancestors) {
        vector<string> row{ancestor, ptr->variableNode->variable};
        output->push_back(row);
      }
      // remove current node from ancestors list
      ancestors->pop_back();
      break;
    }
    case AST::WHILE_NODE: {
      shared_ptr<WhileNode> ptr = dynamic_pointer_cast<WhileNode>(node);
      int stmtNo = stmtNumbers->at(ptr);
      ancestors->push_back(to_string(stmtNo));
      vector<shared_ptr<StmtNode>> stmtList = ptr->stmtList;
      for (shared_ptr<StmtNode> n : stmtList) {
        extractorHelper(n);
      }
      // remove current node from ancestors list
      ancestors->pop_back();
      break;
    }
    case AST::IF_NODE: {
      shared_ptr<IfNode> ptr = dynamic_pointer_cast<IfNode>(node);
      int stmtNo = stmtNumbers->at(ptr);
      ancestors->push_back(to_string(stmtNo));
      vector<shared_ptr<StmtNode>> ifStmtList = ptr->ifStmtList;
      vector<shared_ptr<StmtNode>> elseStmtList = ptr->elseStmtList;
      for (shared_ptr<StmtNode> n : ifStmtList) {
        extractorHelper(n);
      }
      for (shared_ptr<StmtNode> n : elseStmtList) {
        extractorHelper(n);
      }
      // remove current node from ancestors list
      ancestors->pop_back();
      break;
    }
    default:
      break;
  }
}

void ModifiesSRelationExtractor::insertCallsForModifiesS() {
  auto mappedCallNodesToProcedures =
      extractCallNodesFromProcedures(programNode);
  for (auto pair : mappedCallNodesToProcedures) {
    vector<shared_ptr<CallNode>> listOfCallNodes = pair.second;
    for (auto callNode : listOfCallNodes) {
      int stmtNo = stmtNumbers->at(callNode);
      extractCallStmtRelationshipsToOutput(stmtNo, callNode,
                                           proceduresToModifiedVarsMap,
                                           mappedCallNodesToProcedures, output);
    }
  }
  insertCallsInIfAndWhileForModifiesS();
}

void ModifiesSRelationExtractor::insertCallsInIfAndWhileForModifiesS() {
  extractCallStmtRelationshipsWithIfAndWhileToOutput(
      programNode, proceduresToModifiedVarsMap, ifWhileStmtNoToModifiedVarsMap,
      output);
}

void ModifiesSRelationExtractor::initIfAndWhileStmtNoToModifiedVarsMap() {
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
          for (auto n : whileNode->stmtList) {
            queue.push(n);
          }
          break;
        }

        case AST::ASSIGN_NODE: {
          shared_ptr<AssignNode> assignNode =
              dynamic_pointer_cast<AssignNode>(nodeEntry);
          uniqueVarList->insert(assignNode->variableNode->variable);
          break;
        }

        case AST::READ_NODE: {
          shared_ptr<ReadNode> readNode =
              dynamic_pointer_cast<ReadNode>(nodeEntry);
          uniqueVarList->insert(readNode->variableNode->variable);
          break;
        }
        default:
          break;
      }
    }
    ifWhileStmtNoToModifiedVarsMap->insert(
        make_pair(to_string(stmtNo), uniqueVarList));
  }
}

void ModifiesSRelationExtractor::save(shared_ptr<ExtractorResult> result) {
  shared_ptr<RelationResult> modifiesSResult =
      static_pointer_cast<RelationResult>(result);
  for (auto entry : *modifiesSResult->getResult()) {
    dataModifier->saveModifiesS(entry);
  }
}
}  // namespace DE