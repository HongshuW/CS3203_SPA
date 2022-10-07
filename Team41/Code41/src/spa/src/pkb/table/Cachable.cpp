//
// Created by hongshu wang on 7/10/22.
//

#include "Cachable.h"

bool Cachable::isQueried() {
    return queried;
}

void Cachable::setQueried(bool status) {
    queried = status;
}

void Cachable::clearCache() {
    setQueried(false);
}
