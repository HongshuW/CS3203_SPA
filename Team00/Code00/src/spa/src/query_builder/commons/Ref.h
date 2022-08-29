//
// Created by Xingchen Lin on 29/8/22.
//

#ifndef SPA_REF_H
#define SPA_REF_H

#pragma once

#include <iostream>
#include "Synonym.h"
#include "Underscore.h"
#include <unordered_set>
#include <variant>

namespace QB {
    using Ref = std::variant<Synonym, Underscore, int>;
}

#endif //SPA_REF_H
