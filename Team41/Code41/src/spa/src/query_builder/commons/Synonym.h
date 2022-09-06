//
// Created by Xingchen Lin on 28/8/22.
//

#ifndef SPA_SYNONYM_H
#define SPA_SYNONYM_H

#pragma once

#include <iostream>
#include <optional>
#include <regex>

namespace QB {
    class Synonym {
    public:
        std::string synonym;
        explicit Synonym(std::string synonym);
        static bool isValidSynonym(std::string str);

        bool operator==(const Synonym& synonym1) const { return synonym == synonym1.synonym; }

        // For printing
        friend std::ostream& operator<<(std::ostream& os, Synonym const& syn) {
            os << syn.synonym;
            return os;
        }
    };
}

#endif //SPA_SYNONYM_H
