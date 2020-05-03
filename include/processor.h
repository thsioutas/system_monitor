#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

 private:
  float previdle_ = 0;
  float previowait_ = 0;
  float prevuser_ = 0;
  float prevnice_ = 0;
  float prevsystem_ = 0;
  float previrq_ = 0;
  float prevsoftirq_ = 0;
  float prevsteal_ = 0;
};

#endif