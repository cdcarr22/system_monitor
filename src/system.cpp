#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    vector<int> process_ids = LinuxParser::Pids();
    //Got from Udacity Help
    processes_.clear();
    for (int a : process_ids) {
        Process p;
        float cpuu;
        std::string cmd = LinuxParser::Command(a);
        float up = LinuxParser::UpTime(a);
        std::string user = LinuxParser::User(a);
        std::string ram = LinuxParser::Ram(a);

        p.setCommand(cmd);
        p.setUptime(up);
        p.setPid(a);
        p.setUser(user);
        p.setRam(ram);

        long seconds = LinuxParser::UpTime() - p.UpTime();
        long totaltime = LinuxParser::ActiveJiffies(a);
        //Got try/catch from Udacity Help section
        try {
            cpuu = float(totaltime) / float(seconds);
        } catch(...) {
            cpuu = 0.0;
        }

        p.setCpuUsage(cpuu);

        processes_.push_back(p);

    };
    std::sort(processes_.rbegin(),processes_.rend());
    return processes_;
 }

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

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
