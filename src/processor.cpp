#include "processor.h"
#include <sstream>

using std::string; 

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{  
    string line, key;
    std::ifstream filestream(LinuxParser::kProcDirectory+LinuxParser::kStatFilename);
    if (filestream.is_open()) { // Code help taken from mentor answer and stack overflow
        while (std::getline(filestream, line)) {
            std::replace(line.begin(), line.begin()+4, ' ', '_');
            std::istringstream linestream(line);
            while (linestream >> key) {
                if (key == "cpu_") {
                    linestream >> user_ >> nice_ >> system_ >> idle_ >> iowait_ >> irq_ >> softirq_ >> steal_ >> guest_ >> guest_nice_;
                }
            }
        }
    }
  
  idle = idle_ + iowait_;
  nonIdle = user_ + nice_ + system_ + irq_ + softirq_ + steal_;
  total = idle + nonIdle;
  
  totald = total - prevTotal;
  idled = idle - prevIdle;
  prevTotal = total;
  prevIdle = idle;

  return (totald - idled)/totald;
}