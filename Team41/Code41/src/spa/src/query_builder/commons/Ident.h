//
// Created by Xingchen Lin on 4/9/22.
//

#ifndef SPA_IDENT_H
#define SPA_IDENT_H

#pragma once

#include <iostream>
#include <optional>
#include <regex>
#include "utils/Utils.h"

using namespace std;

namespace QB {
    class Ident {
    public:
        string identStr;
        Ident(string identStr);

        bool operator==(const Ident& ident1) const { return identStr == ident1.identStr; }

        // For printing
        friend std::ostream& operator<<(std::ostream& os, Ident const& ident) {
            os << ident.identStr;
            return os;
        }
    };
}

#endif //SPA_IDENT_H
