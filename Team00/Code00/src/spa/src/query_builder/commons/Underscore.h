//
// Created by Xingchen Lin on 30/8/22.
//

#ifndef SPA_UNDERSCORE_H
#define SPA_UNDERSCORE_H

#pragma once
#include <iostream>

namespace QB {
    class Underscore {
    public:
        bool operator==(const Underscore&) const { return true; }
        friend std::ostream& operator<<(std::ostream& os, Underscore const& underscore) {
            os << "_";
            return os;
        }
    };
}  // namespace QE

#endif //SPA_UNDERSCORE_H
