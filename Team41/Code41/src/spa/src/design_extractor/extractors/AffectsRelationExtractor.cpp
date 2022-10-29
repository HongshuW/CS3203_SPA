//
// Created by Xingchen Lin on 28/10/22.
//

#include "AffectsRelationExtractor.h"

#include "NextRelationExtractor.h"
#include "design_extractor/results/QueryTimeResult.h"

namespace DE {
AffectsRelationExtractor::AffectsRelationExtractor(
    shared_ptr<ProgramNode> programNode)
    : AffectsCommonExtractor(std::move(programNode)) {}

bool AffectsRelationExtractor::isModified(string variable, int stmtNo) {
  shared_ptr<StmtNode> stmtNode =
      dynamic_pointer_cast<StmtNode>(lineNoToNodePtrMap->at(stmtNo));
  NodeType nodeType = ASTUtils::getNodeType(stmtNode);
  switch (nodeType) {
    case NodeType::ASSIGN_NODE: {
      shared_ptr<AssignNode> assignNode =
          dynamic_pointer_cast<AssignNode>(stmtNode);
      return assignNode->variableNode->variable == variable;
    }
    case NodeType::READ_NODE: {
      shared_ptr<ReadNode> readNode = dynamic_pointer_cast<ReadNode>(stmtNode);
      return readNode->variableNode->variable == variable;
    }
    case NodeType::CALL_NODE: {
      shared_ptr<CallNode> callNode = dynamic_pointer_cast<CallNode>(stmtNode);
      string procedure = callNode->procedureName;
      unordered_set<string> modifiedVariables = *(modifiesPMap->at(procedure));
      return modifiedVariables.find(variable) != modifiedVariables.end();
    }
    default: {
      return false;
    }
  }
}

bool AffectsRelationExtractor::checkNoWildcardDFS(int start, int end, CFG cfg) {
  shared_ptr<AssignNode> startNode =
      dynamic_pointer_cast<AssignNode>(lineNoToNodePtrMap->at(start));
  string modifiedVar = startNode->variableNode->variable;
  // use dfs because path from start to end matters
  unordered_set<int> visited = unordered_set<int>();
  stack<int> dfsStack = stack<int>();
  dfsStack.push(start);
  // dfs
  while (!dfsStack.empty()) {
    // get first element from stack
    int stmt = dfsStack.top();
    dfsStack.pop();
    unordered_set<int> children = cfg.cfg->find(stmt)->second;
    for (int child : children) {
      if (child == end) {
        // if child is the goal, return whether child uses modifiedVar
        shared_ptr<AssignNode> endNode =
            dynamic_pointer_cast<AssignNode>(lineNoToNodePtrMap->at(end));
        unordered_set<string> usedVariables =
            getVariablesFromExprNode(endNode->expressionNode);
        return usedVariables.find(modifiedVar) != usedVariables.end();
      } else {
        // if child is not goal but modifies modifiedVar, search other paths
        if (isModified(modifiedVar, child)) {
          visited.insert(child);
          continue;
        }
      }
      if (visited.find(child) == visited.end()) {
        // if child hasn't been visited, push it to the stack
        dfsStack.push(child);
        visited.insert(child);
      }
    }
  }
  return false;
}

shared_ptr<vector<string>> AffectsRelationExtractor::extractNoWildcard(
    StmtNoArgs args) {
  int start = args.getStartStmtNo();
  int end = args.getEndStmtNo();
  shared_ptr<vector<string>> output =
      make_shared<vector<string>>(vector<string>());
  if (!areArgsValid(start, end)) {
    return output;
  }
  shared_ptr<ProcedureNode> procedureNode = stmtNoToProcMap->at(start);
  CFG cfg = procCFGMap->at(procedureNode);
  if (checkNoWildcardDFS(start, end, cfg)) {
    output->push_back(to_string(start));
    output->push_back(to_string(end));
  }
  return output;
}

shared_ptr<vector<string>> AffectsRelationExtractor::extractWithStartGivenDFS(
    CFG cfg, int start) {
  shared_ptr<vector<string>> output =
      make_shared<vector<string>>(vector<string>());
  shared_ptr<AssignNode> startNode =
      dynamic_pointer_cast<AssignNode>(lineNoToNodePtrMap->at(start));
  string modifiedVar = startNode->variableNode->variable;
  // use dfs because path from start to end matters
  unordered_set<int> visited = unordered_set<int>();
  stack<int> dfsStack = stack<int>();
  dfsStack.push(start);
  // dfs
  while (!dfsStack.empty()) {
    // get first element from stack
    int stmt = dfsStack.top();
    dfsStack.pop();
    unordered_set<int> children = cfg.cfg->find(stmt)->second;
    for (int child : children) {
      if (visited.find(child) != visited.end()) {
        continue;
      }
      shared_ptr<StmtNode> childNode = lineNoToNodePtrMap->at(child);
      NodeType nodeType = ASTUtils::getNodeType(childNode);
      if (nodeType == NodeType::ASSIGN_NODE) {
        // if child is an assign statement, and it uses modifiedVar, add it to
        // the output
        shared_ptr<AssignNode> assignNode =
            dynamic_pointer_cast<AssignNode>(childNode);
        unordered_set<string> usedVariables =
            getVariablesFromExprNode(assignNode->expressionNode);
        if (usedVariables.find(modifiedVar) != usedVariables.end()) {
          output->push_back(to_string(child));
        }
      }
      if (isModified(modifiedVar, child)) {
        // if child modifies modifiedVar, stop searching the path
        visited.insert(child);
        continue;
      }
      if (visited.find(child) == visited.end()) {
        // if child hasn't been visited, push it to the stack
        dfsStack.push(child);
        visited.insert(child);
      }
    }
  }
  return output;
}

void AffectsRelationExtractor::extractWithEndBTHelper(
    CFG cfg, int stmt, shared_ptr<vector<string>> output,
    shared_ptr<unordered_set<string>> usedVariables,
    shared_ptr<unordered_set<int>> visited) {
  if (usedVariables->empty()) {
    return;
  }
  unordered_set<int> children = cfg.reversedCfg->find(stmt)->second;
  for (int child : children) {
    if (visited->find(child) != visited->end()) {
      // if child has been visited, search other paths
      continue;
    }
    shared_ptr<StmtNode> childNode = lineNoToNodePtrMap->at(child);
    NodeType nodeType = ASTUtils::getNodeType(childNode);
    if (nodeType == NodeType::ASSIGN_NODE) {
      shared_ptr<AssignNode> assignNode =
          dynamic_pointer_cast<AssignNode>(childNode);
      string modifiedVar = assignNode->variableNode->variable;
      auto iterator = usedVariables->find(modifiedVar);
      if (iterator != usedVariables->end()) {
        // if child (assign) modifies any variable in usedVariables, add it to
        // the output
        output->push_back(to_string(child));
        usedVariables->erase(iterator);
        visited->insert(child);
        // continue to search the path
        extractWithEndBTHelper(cfg, child, output, usedVariables, visited);
        usedVariables->insert(modifiedVar);
      } else {
        // if child (assign) doesn't modify any variable in usedVariables,
        // continue searching the path
        visited->insert(child);
        extractWithEndBTHelper(cfg, child, output, usedVariables, visited);
      }
    } else if (nodeType == NodeType::READ_NODE) {
      shared_ptr<ReadNode> readNode = dynamic_pointer_cast<ReadNode>(childNode);
      string modifiedVar = readNode->variableNode->variable;
      auto iterator = usedVariables->find(modifiedVar);
      if (iterator != usedVariables->end()) {
        // if child (read) modifies any variable in usedVariables, remove this
        // variable
        usedVariables->erase(iterator);
        visited->insert(child);
        // continue to search the path
        extractWithEndBTHelper(cfg, child, output, usedVariables, visited);
        usedVariables->insert(modifiedVar);
      } else {
        // if child (read) doesn't modify any variable in usedVariables,
        // continue searching the path
        visited->insert(child);
        extractWithEndBTHelper(cfg, child, output, usedVariables, visited);
      }
    } else if (nodeType == NodeType::CALL_NODE) {
      // TODO: try to optimize this part
      shared_ptr<CallNode> callNode = dynamic_pointer_cast<CallNode>(childNode);
      string procedure = callNode->procedureName;
      shared_ptr<unordered_set<string>> modifiedVariables =
          modifiesPMap->at(procedure);
      unordered_set<string> usedVarCopy = unordered_set<string>();
      unordered_set<string> setUnion = unordered_set<string>();
      for (string usedVar : *usedVariables) {
        usedVarCopy.insert(usedVar);
      }
      for (string usedVar : usedVarCopy) {
        if (modifiedVariables->find(usedVar) != modifiedVariables->end()) {
          // if used variable is modified, remove it from usedVariables
          usedVariables->erase(usedVar);
          setUnion.insert(usedVar);
        }
      }
      visited->insert(child);
      // continue to search the path
      extractWithEndBTHelper(cfg, child, output, usedVariables, visited);
      // add back variables removed
      for (string removedVar : setUnion) {
        usedVariables->insert(removedVar);
      }
    } else if (nodeType == NodeType::IF_NODE ||
               nodeType == NodeType::WHILE_NODE) {
      extractWithEndBTHelper(cfg, child, output, usedVariables, visited);
    } else {
      visited->insert(child);
      extractWithEndBTHelper(cfg, child, output, usedVariables, visited);
    }
  }
}

shared_ptr<vector<string>>
AffectsRelationExtractor::extractWithEndGivenBackTracking(CFG cfg, int end) {
  shared_ptr<vector<string>> output =
      make_shared<vector<string>>(vector<string>());
  shared_ptr<AssignNode> endNode =
      dynamic_pointer_cast<AssignNode>(lineNoToNodePtrMap->at(end));
  shared_ptr<unordered_set<string>> usedVariables =
      make_shared<unordered_set<string>>(
          getVariablesFromExprNode(endNode->expressionNode));
  shared_ptr<unordered_set<int>> visited =
      make_shared<unordered_set<int>>(unordered_set<int>());
  // back tracking
  extractWithEndBTHelper(cfg, end, output, usedVariables, visited);
  return output;
}

shared_ptr<vector<string>> AffectsRelationExtractor::extractOneWildcard(
    StmtNoArgs args, bool isStartGiven) {
  int startingPoint =
      isStartGiven ? args.getStartStmtNo() : args.getEndStmtNo();
  NodeType nodeType = ASTUtils::getNodeType(lineNoToNodePtrMap->at(startingPoint));
  bool invalidArg = (stmtNoToProcMap->count(startingPoint) == 0
          && startingPoint != 0) || nodeType != NodeType::ASSIGN_NODE;
  if (invalidArg) {
    return make_shared<vector<string>>(vector<string>());
  }
  shared_ptr<ProcedureNode> procedureNode = stmtNoToProcMap->at(startingPoint);
  CFG cfg = procCFGMap->at(procedureNode);
  return isStartGiven ? extractWithStartGivenDFS(cfg, startingPoint)
                      : extractWithEndGivenBackTracking(cfg, startingPoint);
}

bool AffectsRelationExtractor::areArgsValid(int start, int end) {
  NodeType startType = ASTUtils::getNodeType(lineNoToNodePtrMap->at(start));
  NodeType endType = ASTUtils::getNodeType(lineNoToNodePtrMap->at(end));
  bool invalidStartArg = stmtNoToProcMap->count(start) == 0 && start != 0;
  bool invalidEndArg = stmtNoToProcMap->count(end) == 0 && end != 0;

  if (startType != NodeType::ASSIGN_NODE || endType != NodeType::ASSIGN_NODE) {
    return false;
  } else if (invalidStartArg || invalidEndArg) {
    return false;
  } else {
    if (stmtNoToProcMap->count(start) != 0 &&
        stmtNoToProcMap->count(end) != 0) {
      bool startAndEndSameProcedure =
          stmtNoToProcMap->at(start) == stmtNoToProcMap->at(end);
      if (!startAndEndSameProcedure) {
        return false;
      }
    }
  }
  return true;
}

shared_ptr<ExtractorResult> AffectsRelationExtractor::extract(StmtNoArgs args) {
  if (args.startAndEndExists()) {
    return make_shared<QueryTimeResult>(extractNoWildcard(args));
  }
  if (args.startExistsOnly()) {
    return make_shared<QueryTimeResult>(extractOneWildcard(args, true));
  }
  if (args.endExistsOnly()) {
    return make_shared<QueryTimeResult>(extractOneWildcard(args, false));
  }
  return make_shared<QueryTimeResult>(make_shared<vector<string>>());
}

shared_ptr<ExtractorResult> AffectsRelationExtractor::extractAllRelations() {
  generateAffectsTable();

  shared_ptr<list<vector<string>>> output = make_shared<list<vector<string>>>();
  for (auto& entry : *affectsTable) {
    output->push_back(entry);
  }

  return make_shared<RelationResult>(output);
}
}  // namespace DE