//
// Created by Tan Wei En on 3/9/22.
//

#ifndef SPA_DESIGNEXTRACTOR_H
#define SPA_DESIGNEXTRACTOR_H

#include "ast/ProgramNode.h"

class DesignExtractor {
private:
    ProgramNode programNode;
public:
    DesignExtractor(ProgramNode programNode);
};

#endif //SPA_DESIGNEXTRACTOR_H