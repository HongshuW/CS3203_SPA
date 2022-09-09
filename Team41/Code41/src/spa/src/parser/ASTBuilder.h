//
// Created by Xingchen Lin on 8/9/22.
//

#ifndef SPA_ASTBUILDER_H
#define SPA_ASTBUILDER_H

#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include "AST/ASTNode.h"
#include "Tokenizer.h"

using namespace std;
using namespace AST;

namespace SourceParser {
    class ASTBuilder {
    public:
        /**
         * Takes a filename string and convert it to an AST tree.
         *
         * @param string filename.
         * @return a shared pointer to the root node of the AST tree
         */
        shared_ptr<ASTNode> buildAST (string filename);
    };
}


#endif //SPA_ASTBUILDER_H
