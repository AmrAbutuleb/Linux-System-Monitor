#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  vector<Process> foundProcesses{};
  processes_.clear();
  vector<int> processIds = LinuxParser::Pids();
  for (int p : processIds) {
    Process process{p};
    foundProcesses.push_back(process);
  }
  sort(foundProcesses.begin(), foundProcesses.end(),
       [](Process& p1, Process& p2) {
         return (p2.CpuUtilization() < p1.CpuUtilization());
       });
  processes_ = foundProcesses;

  return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return string(LinuxParser::Kernel());}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }