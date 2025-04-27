#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>

#include "linux_parser.h"
//#include <filesystem>

using std::stof;
using std::string;
using std::to_string;
using std::vector;
//namespace fs =  std::filesystem;
std::vector<long> LinuxParser::jiffies = {}; 

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

  
//   for (const auto& entry : fs::directory_iterator(kProcDirectory))
//   {
//     if (fs::is_directory(entry))
//     {
//       string dirName = entry.path().file_name();
//       if(std::all_of(dirName.begin(), dirName.end(),::isdigit))
//       {
//         pids.push_back(stoi(dirName));
//       }

//     }
//   }
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
    std::string line, key;
    long memTotal = 0, memFree = 0, value;
    std::ifstream filestream("/proc/meminfo");
    
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            linestream >> key >> value;
            if (key == "MemTotal:") {
                memTotal = value;
            } else if (key == "MemFree:") {
                memFree = value;
            }
            if (memTotal > 0 && memFree > 0) {
                break;
            }
        }
    }
    
    if (memTotal == 0) return 0.0f; // avoid division by zero
    return (float)(memTotal - memFree) / memTotal;
}
// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
    std::string line;
    long uptime_seconds = 0;
    std::ifstream filestream("/proc/uptime");
    
    if (filestream.is_open()) {
        if (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            linestream >> uptime_seconds;
        }
    }
    
    return uptime_seconds;
}


// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  
  string line,cpu;
  //user,nice,system,idle,iowait,irq,softirq,steal,guest,guest_nice;
  string value;
  long lValue;

  long total = 0;

  LinuxParser::jiffies.clear();

  std::ifstream fileStream (kProcDirectory + kStatFilename);
  if (fileStream.is_open())
  {
    std::getline(fileStream,line);
    std::istringstream lineStream(line);
    lineStream >> cpu;
    while(lineStream >> value)
    {
      lValue = std::stol(value);
      total += lValue;
      LinuxParser::jiffies.push_back(lValue);
    }

  }
  return total; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  LinuxParser::Jiffies();  // Update jiffies

  if (jiffies.size() < 8) {
    // Not enough elements, avoid crash
    return 0;
  }

  return jiffies[0] + jiffies[1] + jiffies[2]
       + jiffies[5] + jiffies[6] + jiffies[7];
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  LinuxParser::Jiffies();  // Update jiffies

  if (jiffies.size() < 5) {
    return 0;
  }

  return jiffies[3] + jiffies[4];
}


// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 

  string line,cpu;
  string value;

  long total = 0;

  std::vector<string> jiffies__;

  std::ifstream fileStream (kProcDirectory + kStatFilename);
  if (fileStream.is_open())
  {
    std::getline(fileStream,line);
    std::istringstream lineStream(line);
    lineStream >> cpu;
    while(lineStream >> value)
    {
      jiffies__.push_back(value);
    }

  }
  return jiffies__; 


 }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 

  string key;
  string value;
  string line;


  std::ifstream fileStream (kProcDirectory + kStatFilename);
  if (fileStream.is_open())
  {

    while(std::getline(fileStream,line))
    {
      std::istringstream lineStream(line);
      while(lineStream >> key)
      {
        if (key == "processes" )
        {
          lineStream >> value;
          return stoi(value);
        }
      }
    }   
  }
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 

  string key;
  string value;
  string line;


  std::ifstream fileStream (kProcDirectory + kStatFilename);
  if (fileStream.is_open())
  {

    while(std::getline(fileStream,line))
    {
      std::istringstream lineStream(line);
      while(lineStream >> key)
      {
        if (key == "procs_running" )
        {
          lineStream >> value;
          return stoi(value);
        }
      }
    }   
  }
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string cmd_line;
  std::ifstream filestream(kProcDirectory+to_string(pid)+kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, cmd_line);
    return cmd_line;
  }
  return cmd_line;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
 string line, key;
  int value = 0;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        break;
      }
    }
  }
  return to_string(value / 1000);  // convert from kB to MB
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line, key, uid;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> uid;
      if (key == "Uid:") {
        break;
      }
    }
  }
  return uid;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line, key, value, x, user_name;
  string uid = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> x >> value; 
      if (value == uid) {
        user_name = key;
        return user_name;
      }
    }
  } 
  return user_name; }

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  long uptime_process{0};
  string line, uptime;
  
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
    if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      for (int i = 0; i<22; ++i){
      linestream >> uptime;
      }
    }
  uptime_process = LinuxParser::UpTime() - std::stol(uptime)/sysconf(_SC_CLK_TCK);
  return uptime_process;
}

// Read and return the CPU usage of a process
// return the values in seconds
vector<float> LinuxParser::CpuUtilization(int pid) {
  vector<float> cpuValues{};
  string line;
  float time = 0;
  string value;
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      for (int i = 1; i <= kStarttime_; i++) {
        linestream >> value;
        // read CPU time spent in user mode - utime #14
        // read CPU time spent in kernel mode - stime #15
        // read Waited for children's CPU time spent in user mode - cutime #16
        // read Waited for children's CPU time spent in kernel mode- cstime #17
        // read the time when process started - starttime #22
        if (i == kUtime_ || i == kStime_ || i == kCutime_ || i == kCstime_ ||
            i == kStarttime_) {
          // read in clock ticks and convert to seconds
          // devide by clock ticks per second
          time = std::stof(value) / sysconf(_SC_CLK_TCK);
          cpuValues.push_back(time);
        }
      }
    }
  }
  return cpuValues;
}