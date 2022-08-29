//
// Created by Xingchen Lin on 28/8/22.
//

#ifndef SPA_DESIGNENTITY_H
#define SPA_DESIGNENTITY_H

#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace QB {
    enum class DesignEntity {
        STMT,
        READ,
        PRINT,
        CALL,
        WHILE,
        IF,
        ASSIGN,
        VARIABLE,
        CONSTANT,
        PROCEDURE,
    };

    DesignEntity getDesignEntity(std::string&);
    std::string getDesignEntityString(DesignEntity);

}  // namespace QE

#endif //SPA_DESIGNENTITY_H
