#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string ElapsedTime(long int time);
std::string Pad(std::string s, char c);// TODO: See src/format.cpp
};                                    // namespace Format

#endif