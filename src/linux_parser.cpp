#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include <filesystem>

using std::stof;
using std::string;
using std::to_string;
using std::vector;
namespace fs =  std::filesystem;

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

  
  for (const auto& entry : fs::directory_iterator(kProcDirectory))
  {
    if (fs::is_directory(entry))
    {
      string dirName = entry.path().file_name();
      if(std::all_of(dirName.begin(), dirName.end(),::isdigit))
      {
        pids.push_back(stoi(dirName));
      }

    }
  }
  // DIR* directory = opendir(kProcDirectory.c_str());
  // struct dirent* file;
  // while ((file = readdir(directory)) != nullptr) {
  //   // Is this a directory?
  //   if (file->d_type == DT_DIR) {
  //     // Is every character of the name a digit?
  //     string filename(file->d_name);
  //     if (std::all_of(filename.begin(), filename.end(), isdigit)) {
  //       int pid = stoi(filename);
  //       pids.push_back(pid);
  //     }
  //   }
  // }
  // closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string memName;
  string size;
  string unit;
  int memTotal;
  int memFree;
  int retValue;
  std::ifstream filestream(kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> memName >> size >> unit) {
        if (memName == "memtotal") {
          memTotal = std::stoi(size);
        }
        else if (memName == "memfree") {
          memFree = std::stoi(size);
        }

      }
    }
    retValue = (static_cast<float>(memTotal) - static_cast<float> (memFree)) / static_cast<float>(memTotal);
  }
  return retValue;

 }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string susbend_;
  string idle_;
  string value;
  string line;
  std::ifstream filestream(kProcDirectory+kUptimeFilename);
  if (filestream.is_open()) 
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> susbend_ >> idle_;
  } 
  return std::stoi(susbend_ + idle_);
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

  (void)LinuxParser::Jiffies();


  return jiffies[0]+ jiffies[1]+ jiffies[2]
          +jiffies[5] + jiffies[6] + jiffies[7];
  
 }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 

  (void)LinuxParser::Jiffies();
  return jiffies[3]+ jiffies[4];

}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 

  string line,cpu;
  string value;

  long total = 0;

  std::vector<string> jiffies;

  std::ifstream fileStream (kProcDirectory + kStatFilename);
  if (fileStream.is_open())
  {
    std::getline(fileStream,line);
    std::istringstream lineStream(line);
    lineStream >> cpu;
    while(lineStream >> value)
    {
      jiffies.push_back(value);
    }

  }
  return jiffies; 


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


  std::ifstream fileStream (kProcDirectory + kMeminfoFilename);
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

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
