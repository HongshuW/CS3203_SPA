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
#include "query_builder/constants/CommonConstants.h"

namespace QB {
    class Synonym {
    public:
        std::string synonym;
        explicit Synonym(const std::string& synonym);

        bool operator==(const Synonym& synonym1) const;
    };
}

#endif //SPA_SYNONYM_H
