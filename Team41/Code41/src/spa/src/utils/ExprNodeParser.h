//
// Created by Xingchen Lin on 10/9/22.
//

#ifndef SPA_EXPRNODEPARSER_H
#define SPA_EXPRNODEPARSER_H

#include "AST/ExprNode.h"
#include <vector>
#include <string>
#include <unordered_set>
#include <stack>

using namespace AST;
using namespace std;

class ExprNodeParser {
private:
    shared_ptr<ExprNode> root;
    vector<string> tokens;
    unsigned int currIdx;

    string peek();
    string pop();
    bool match(string s);
    bool equals(string s1, string s2);
    int getPriority(string s);
    void buildTree(string opt, shared_ptr<stack<shared_ptr<ExprNode>>> opd);

public:
    ExprNodeParser(vector<string>& tokens);
    shared_ptr<ExprNode> parse();
};


#endif //SPA_EXPRNODEPARSER_H
