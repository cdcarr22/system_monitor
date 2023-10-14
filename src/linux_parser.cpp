#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include <unistd.h>
#include "linux_parser.h"
#include "format.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::stol;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
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
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
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
  string line;
  string key;
  string value;
  float total = 0;
  float available = 0;
  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key;
        if (key == "MemTotal") {
          linestream >> total;
        }
        else if (key == "MemAvailable") {
          linestream >> available;
          
        };
      
    }

  }
   
      return (total-available)/total;
  }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  long uptime;
  long idletime;
  std::ifstream stream(kProcDirectory+kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
    return uptime;
    
  };
  return uptime;
 }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
 }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string line;
  string value;
  vector<std::string> values;
  long totaltime = 0, cutime = 0, cstime = 0, utime = 0, stime = 0;
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream,line);
    std::istringstream linestream(line);

    while (linestream >> value) {
      values.push_back(value);
    }    
  }
  utime = stol(values[13]);
  stime = stol(values[14]);
  cutime = stol(values[15]);
  cstime = stol(values[16]);

  totaltime = float(utime + stime +cstime + cutime);

  return totaltime/sysconf(_SC_CLK_TCK);
 }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  auto activej = CpuUtilization();

  auto total_aj = stol(activej[CPUStates::kUser_])+stol(activej[CPUStates::kNice_])+stol(activej[CPUStates::kSystem_])+
                  stol(activej[CPUStates::kIRQ_])+stol(activej[CPUStates::kSoftIRQ_])+stol(activej[CPUStates::kSteal_])+
                  stol(activej[CPUStates::kGuest_])+stol(activej[CPUStates::kGuestNice_]);
  return total_aj;
 }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  auto idlej = CpuUtilization();

  auto total_i = stol(idlej[CPUStates::kIdle_])+stol(idlej[CPUStates::kIOwait_]);
  return total_i;
 }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  string key;
  string value;
  vector<std::string> values;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream,line);
    std::istringstream linestream(line);
    linestream >> key;

    while (linestream >> value) {
      values.push_back(value);
    }    
  }
  return values;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  long procs;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> procs) {
        if (key == "processes") {
          linestream >> procs;
          return procs;
        }
      }

    }
  }
  return procs;
 }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  int running;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> running) {
        if (key == "procs_running") {
          linestream >> running;
          return running;
        }
      }
    }
  }
  return running;
 }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  string cmd;
  std::ifstream stream(kProcDirectory+std::to_string(pid)+kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cmd;
    return cmd;
    
  };
  return cmd;
 }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line;
  string key, value, ext, mem;

  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value >> ext;
        if (key == "VmSize") {
          mem = value;
          break;
        }      
    }

  }
   
      return std::to_string(stol(mem)/1024);
 }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key;
        if (key == "Uid") {
          linestream >> value;
        };      
    }

  }
   
      return value;
 }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line, uu;
  string value1, value2, value3;
  string name = "";

  uu = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> value1 >> value2 >> value3;
        if (uu == value3) {
          name = value1;
          break;
        };      
    }

  }
   
    return name;
 }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line;
  string value;
  vector<std::string> values;
  long time = 0;
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream,line);
    std::istringstream linestream(line);

    while (linestream >> value) {
      values.push_back(value);
    }    
  }
  time = stol(values[21]) / sysconf(_SC_CLK_TCK);
  return time;
 }
