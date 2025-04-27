#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : processId_(pid) {
}

// Return this process's ID
int Process::Pid() const { return processId_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const {  // read values from filesystem
  long uptime = LinuxParser::UpTime();
  vector<float> val = LinuxParser::CpuUtilization(Pid());
  // only if the values could be read sucessfully
  if (val.size() == 5) {
    // add utime, stime, cutime, cstime (they are in seconds)
    float totaltime =
        val[kUtime_] + val[kStime_] + val[kCutime_] + val[kCstime_];
    float seconds = uptime - val[kStarttime_];
    // calculate the processes CPU usage
    cpuUsage_ = totaltime / seconds;
  } else{
    cpuUsage_ = 0; }

return cpuUsage_;
}

// Return the command that generated this process
string Process::Command() { 
  
  command_ = LinuxParser::Command(Pid());
  return command_; 
}

// Return this process's memory utilization
string Process::Ram() { 
  
  // read kB value from file
  string val = LinuxParser::Ram(Pid());
  // convert into MB
  try {
    long conv = std::stol(val) / 1000;
    ram_ = std::to_string(conv);
  } catch (const std::invalid_argument& arg) {
    ram_ = "0";
  }
  
  
  return ram_; 
}

// Return the user (name) that generated this process
string Process::User() { 
  
  // read kB value from file
  string val = LinuxParser::Ram(Pid());
  // convert into MB
  try {
    long conv = std::stol(val) / 1000;
    ram_ = std::to_string(conv);
  } catch (const std::invalid_argument& arg) {
    ram_ = "0";
  }
  
  return user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() { 
  
  
   uptime_ = LinuxParser::UpTime(Pid());
  return uptime_; }


