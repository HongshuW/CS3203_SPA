//
// Created by Xingchen Lin on 28/8/22.
//

#ifndef SPA_SYNONYM_H
#define SPA_SYNONYM_H

#pragma once

#include <iostream>
#include <optional>
#include <regex>
#include "utils/Utils.h"

namespace QB {
    class Synonym {
    public:
        std::string synonym;
        explicit Synonym(std::string synonym);

        bool operator==(const Synonym& synonym1) const;

        // For printing
        friend std::ostream& operator<<(std::ostream& os, Synonym const& syn);
    };
}

#endif //SPA_SYNONYM_H
