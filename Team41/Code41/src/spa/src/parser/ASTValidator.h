//
// Created by Tan Wei En on 3/10/22.
//

#ifndef SPA_ASTVALIDATOR_H
#define SPA_ASTVALIDATOR_H

#include "AST/CallNode.h"
#include "AST/ProgramNode.h"
#include "constants/ParserConstants.h"
#include "unordered_map"
#include "utils/ErrorMessageFormatter.h"

using namespace std;

namespace SourceParser {
class ASTValidator {
 private:
  unordered_set<string> procedureNames;
  unordered_map<string, unordered_set<string>> procedureCalls;
  shared_ptr<ProgramNode> ast;

  void validateProcedureNames();
  void validateNodes();
  void validateCyclicDependencies();
  void validateCallNode(const shared_ptr<CallNode>& callNode,
                        const string& procedureName);
  void calls(const string& procedure, const string& calledProcedure);

 public:
  /**
   * Validates an AST
   *
   * @param shared_ptr<ProgramNode> AST.
   * @return true if AST is valid, false otherwise
   */
  void validateAST();
  explicit ASTValidator(shared_ptr<ProgramNode> ast);
};
}  // namespace SourceParser

#endif  // SPA_ASTVALIDATOR_H
