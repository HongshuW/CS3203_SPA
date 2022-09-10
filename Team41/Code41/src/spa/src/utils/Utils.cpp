//
// Created by Xingchen Lin on 9/9/22.
//

#include "Utils.h"

const std::regex NAME_REGEX = std::regex("[a-zA-Z](\\d|[a-zA-Z])*");

bool Utils::isValidName(string str) {
    return regex_match(str, NAME_REGEX);
}

bool Utils::isValidNumber(string str) {
    return str.find_first_not_of("0123456789") == std::string::npos;
}