#ifndef PROCESSOR_H
#define PROCESSOR_H
#include<vector>
#include<string>
using std::string;
using std::vector;

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
  std::vector<long> convertToLong(vector<string> values);
 private:
  float  prevTotalCpuTime = 0.0, prevIdleCpuTime = 0.0;
};

#endif