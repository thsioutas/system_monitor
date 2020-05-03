#include <iostream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "processor.h"

using std::string;
using std::vector;

// Return the aggregate CPU utilization
float Processor::Utilization() {
  // Read cpu = {user, nice, system, idle, iowait, irq, softirq, steal, guest,
  // guest_nice};
  vector<string> cpu = LinuxParser::CpuUtilization();
  if (cpu.size() < 8) return 0;
  float user = stof(cpu[0]);
  float nice = stof(cpu[1]);
  float system = stof(cpu[2]);
  float idle = stof(cpu[3]);
  float iowait = stof(cpu[4]);
  float irq = stof(cpu[5]);
  float softirq = stof(cpu[6]);
  float steal = stof(cpu[7]);

  float PrevIdle = previdle_ + previowait_;
  float Idle = idle + iowait;

  float PrevNonIdle = prevuser_ + prevnice_ + prevsystem_ + previrq_ +
                      prevsoftirq_ + prevsteal_;
  float NonIdle = user + nice + system + irq + softirq + steal;

  float PrevTotal = PrevIdle + PrevNonIdle;
  float Total = Idle + NonIdle;

  // differentiate: actual value minus the previous one
  float totald = Total - PrevTotal;
  float idled = Idle - PrevIdle;

  float CPU_Percentage = (totald - idled) / totald;
  previdle_ = idle;
  previowait_ = iowait;
  prevuser_ = user;
  prevnice_ = nice;
  prevsystem_ = system;
  previrq_ = irq;
  prevsoftirq_ = softirq;
  prevsteal_ = steal;
  return CPU_Percentage;
}