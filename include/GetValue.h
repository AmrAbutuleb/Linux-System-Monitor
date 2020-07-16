#ifndef HELPER_PARSER_H
#define HELPER_PARSER_H

#include <fstream>
#include <regex>
#include <string>

#include "linux_parser.h"

using std::string;
using std::vector;

namespace GetValue {
template <typename G>
G GetValueByKey(const string &filter, const string &filename) {
  string line, key;
  G value;

  std::ifstream stream(LinuxParser::kProcDirectory + filename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == filter) {
          return value;
        }
      }
    }
  }
  return value;
}
  template <typename H>
  H  GetValueByKeyProcess(const string &filter, const string &filename, int pid) {
  string line, key;
  H value;

  std::ifstream stream(LinuxParser::kProcDirectory + std::to_string(pid) + filename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == filter) {
          return value;
        }
      }
    }
  }
  return value;
};

}  // namespace GetValues
#endif