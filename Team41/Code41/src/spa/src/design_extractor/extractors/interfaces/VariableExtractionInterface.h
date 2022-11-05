//
// Created by Nafour on 27/10/22.
//

#ifndef SPA_VARIABLEEXTRACTIONINTERFACE_H
#define SPA_VARIABLEEXTRACTIONINTERFACE_H
#include <memory>
#include <string>
#include <unordered_set>

#include "AST/CondExprNode.h"
#include "AST/ExprNode.h"

using namespace std;
namespace DE {

class VariableExtractionInterface {
 public:
  unordered_set<string> getVariablesFromExprNode(
      const shared_ptr<ExprNode>& exprNode);
  unordered_set<string> getVariablesFromCondExprNode(
      const shared_ptr<CondExprNode>& condExprNode);
};

}  // namespace DE

#endif  // SPA_VARIABLEEXTRACTIONINTERFACE_H
