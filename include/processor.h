#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  float total, idle, nonIdle, totald,idled, prevTotal, prevIdle;

  // TODO: Declare any necessary private members
 private:
  float user_, nice_, system_, idle_, iowait_, irq_, softirq_, steal_, guest_, guest_nice_;
  std::string processor_;
};

#endif