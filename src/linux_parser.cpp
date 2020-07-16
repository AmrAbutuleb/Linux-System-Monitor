#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "GetValue.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::all_of;
using std::replace;
using std::istringstream;
using std::ifstream;
using std::getline;


// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      replace(line.begin(), line.end(), ' ', '_');
      replace(line.begin(), line.end(), '=', ' ');
      replace(line.begin(), line.end(), '"', ' ');
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    getline(stream, line);
    istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float a = GetValue::GetValueByKey<int>(LinuxParser::filterMemTotal, LinuxParser::kMeminfoFilename);
  float b = GetValue::GetValueByKey<int>(LinuxParser::filterMemFree, LinuxParser::kMeminfoFilename);
  return (a-b)/a;
}

// TODO: Read and return the system uptime
long int LinuxParser::UpTime(){ 
  long int UT1;
  string line;
  ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename);
  if (stream.is_open()) {
    getline(stream, line);
    istringstream linestream(line);
    linestream >> UT1;
  }
  return UT1;
}


// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> CpuValues{};
  string line, key;
  string vuser, vnice, vsystem, vidle, viowait, virq, vsoftirq, vsteal, vguest,
      vguest_nice;
  ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);
      while (linestream >> key >> vuser >> vnice >> vsystem >> vidle >>
             viowait >> virq >> vsoftirq >> vsteal >> vguest >> vguest_nice) {
        if (key == "cpu") {
          CpuValues.push_back(vuser);
          CpuValues.push_back(vnice);
          CpuValues.push_back(vsystem);
          CpuValues.push_back(vidle);
          CpuValues.push_back(viowait);
          CpuValues.push_back(virq);
          CpuValues.push_back(vsoftirq);
          CpuValues.push_back(vsteal);
          CpuValues.push_back(vguest);
          CpuValues.push_back(vguest_nice);
          return CpuValues;
        }
      }
    }
  }
  return {};
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int a = GetValue::GetValueByKey<int>(LinuxParser::filterProcesses, LinuxParser::kStatFilename);
  return a;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int a = GetValue::GetValueByKey<int>(LinuxParser::filterRunningProcesses, LinuxParser::kStatFilename);
  return a;
}


float LinuxParser::CpuUtilization(int pid) {
  string key ,line,value ;
    vector<string> data ;
    int Hertz = sysconf(_SC_CLK_TCK);
    std::ifstream filestream(LinuxParser::kProcDirectory+to_string(pid)+LinuxParser::kStatFilename);
    if(filestream.is_open()){
        std::getline(filestream,line);
        std::istringstream linestream(line) ; 
        for (int i = 0; i < 22 ; i++)
        {
            linestream >> value ; 
            data.push_back(value);
        }
    }
//calculations 
int totalTime = std::stoi(data[13])+std::stoi(data[14])+std::stoi(data[15])+std::stoi(data[16]);
int seconds = LinuxParser::UpTime()-(std::stoi(data[21])/Hertz);
float cpuU = (((float)totalTime/(float)Hertz)/(float)seconds);

return cpuU ;
}


// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string cmd;
  string line;
  ifstream stream(LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kCmdlineFilename);
  if (stream.is_open()) {
    getline(stream, line);
    replace(line.begin(), line.end(), ' ', '_');
    istringstream linestream(line);
    linestream >> cmd;
    replace(cmd.begin(), cmd.end(), '_', ' ');
  }
  return cmd;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
 long a = GetValue::GetValueByKeyProcess<long>(LinuxParser::filterProcMem, LinuxParser::kStatusFilename, pid);
 string c = to_string((a/1024));
 return c;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string a = GetValue::GetValueByKeyProcess<string>(LinuxParser::filterUID, LinuxParser::kStatusFilename, pid);
  return a;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);
  string line;
  string key;
  string value;
  string other;
  // find user name for this user ID in /etc/passwd
  std::ifstream filestream(LinuxParser::kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> value >> other >> key) {
        if (key == uid) {
          return value;
        }
      }
    }
  }
  return value;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line;
  long uptime = 0;
  long upTimePid = 0;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      for (int i = 0; i <= 22; i++) {
        linestream >> value;
        if (i == 21) {
            uptime = std::stol(value) / sysconf(_SC_CLK_TCK);
            long upTimePid = LinuxParser::UpTime() - uptime;
            return upTimePid;
        } 
      }
    }
  }
  return upTimePid;
}