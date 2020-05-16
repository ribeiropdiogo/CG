//
// Created by syrayse on 16/05/20.
//
#include "Common.h"

bool isSuffixOf (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}