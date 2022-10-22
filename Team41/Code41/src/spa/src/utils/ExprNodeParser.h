//
// Created by Xingchen Lin on 10/9/22.
//

#ifndef SPA_EXPRNODEPARSER_H
#define SPA_EXPRNODEPARSER_H

#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

#include "AST/ExprNode.h"
#include "parser/constants/ParserConstants.h"

using namespace AST;
using namespace std;
using namespace SourceParser;

class ExprNodeParser {
 private:
  shared_ptr<ExprNode> root;
  vector<string> tokens;
  int currIdx;

  string peek();
  string pop();
  bool match(const string& s);
  int getPriority(const string& s);
  //! bool to indicate whether we are able to build the tree
  bool buildTree(const string& opt,
                 const shared_ptr<stack<shared_ptr<ExprNode>>>& opd);

 public:
  explicit ExprNodeParser(vector<string>& tokens);
  shared_ptr<ExprNode> parse();
};

#endif  // SPA_EXPRNODEPARSER_H
