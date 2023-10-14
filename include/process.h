#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();  
  void setCommand(std::string command) {
    cmd_ = command;
  };
  void setUser(std::string user) {
    user_ = user;
  };
    void setUptime(long up) {
    uptime_ = up;
  };
  void setCpuUsage(float cpu) {
    cpu_utilization_ = cpu;
  };
  void setRam(std::string ram) {
    ram_ = ram;
  };
  void setPid(int id) {
    pid_ = id;
  };
  bool operator<(Process & a);  // TODO: See src/process.cpp


  // TODO: Declare any necessary private members
 private:
  int pid_;
  std::string cmd_;
  long uptime_;
  std::string ram_;
  std::string user_;
  float cpu_utilization_;
};

#endif