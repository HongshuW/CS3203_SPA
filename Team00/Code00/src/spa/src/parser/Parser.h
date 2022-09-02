//
// Created by Tan Wei En on 2/9/22.
//

#include <vector>
#include "ast/ProcedureNode.h"
#include "ast/AssignNode.h"
#include "ast/ASTNode.h"

#ifndef SPA_PARSER_H
#define SPA_PARSER_H

#endif //SPA_PARSER_H

namespace SourceParser {
    class Parser {
    private:
        std::vector<std::string> tokens;
        std::shared_ptr<ProcedureNode> parseProcedure
    public:
        ASTNode parse();
    };
}
