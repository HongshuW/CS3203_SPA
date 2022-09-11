//
// Created by Xingchen Lin on 11/9/22.
//

#ifndef SPA_CONDEXPRPARSER_H
#define SPA_CONDEXPRPARSER_H

#include "AST/CondExprNode.h"
#include "Exceptions.h"
#include "ExprNodeParser.h"
#include "utils/Utils.h"
#include <string>
#include <vector>

using namespace AST;
using namespace std;

namespace SourceParser {

    class CondExprParser {
    private:
        shared_ptr<CondExprNode> root;
        vector<string> tokens;
        unsigned int currIdx;

        string peek();
        string pop();
        bool match(string s);
        bool equals(string s1, string s2);
        bool expect(string s);

        shared_ptr<ExprNode> parseExprNodeForRelExpr(string direction);
        shared_ptr<RelExprNode> parseRelExprNode();

    public:
        CondExprParser(vector<string>& tokens);
        shared_ptr<CondExprNode> parse();
    };

} // SourceParser

#endif //SPA_CONDEXPRPARSER_H
