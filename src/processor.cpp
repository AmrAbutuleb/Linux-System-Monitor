#include "processor.h"
#include <string>
#include <vector>
#include "linux_parser.h"

using std::string;
using std::vector;


vector<long> Processor::convertToLong(vector<std::string> values) {
     vector<long> convertedValues{};

     for (int it = 0; it < (int)values.size(); it++) {
           convertedValues.push_back(std::stol(values[it]));
     }
    return convertedValues;
}
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  vector<long> cpuValues = convertToLong(LinuxParser::CpuUtilization());
  float TotalCpuTime = cpuValues[0]+cpuValues[1]+cpuValues[2]+cpuValues[3]+cpuValues[4]+cpuValues[5]+cpuValues[6]+cpuValues[7];
  float IdleCpuTime = cpuValues[3]+cpuValues[4];
  float diffIdle = IdleCpuTime - prevIdleCpuTime;
  float diffTotal = TotalCpuTime - prevTotalCpuTime;
  float diffUsage = (diffTotal - diffIdle) / diffTotal;
  prevIdleCpuTime = IdleCpuTime;
  prevTotalCpuTime = TotalCpuTime;
  return diffUsage;
}
  
  
