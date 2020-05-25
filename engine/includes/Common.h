/**
 * Defines common functions and transversal to all mechanisms
 * in the engine.
 */
#ifndef GENERATOR_COMMON_H
#define GENERATOR_COMMON_H

#include <string>

/**
 * Verifies is a string is suffix of another.
 *
 * @param fullString Full string-
 * @param ending Suffix to match with.
 *
 * @return true if the string is suffixed, false otherwise.
 */
bool isSuffixOf (std::string const &fullString, std::string const &ending);

#endif
