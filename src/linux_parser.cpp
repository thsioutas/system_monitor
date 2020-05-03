#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>

#include "format.h"
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string key, value;
  string line;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  float mem_total, mem_free;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          mem_total = stof(value);
        } else if (key == "MemFree:") {
          mem_free = stof(value);
        }
      }
    }
  }
  return (mem_total - mem_free) / mem_total;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return stol(uptime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string key, user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  vector<string> cpu_utilization;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> user >> nice >> system >> idle >> iowait >>
             irq >> softirq >> steal) {
        if (key == "cpu") {
          cpu_utilization = {user,   nice, system,  idle,
                             iowait, irq,  softirq, steal};
        }
      }
    }
  }
  return cpu_utilization;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string key, value;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  int processes;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          processes = stoi(value);
        }
      }
    }
  }
  return processes;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string key, value;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  int procs_running;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          procs_running = stoi(value);
        }
      }
    }
  }
  return procs_running;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  return line;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string key, value;
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          int ram = stoi(value) * 0.001;
          return std::to_string(ram);
        }
      }
    }
  }
  return std::to_string(0);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); }

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string key, value, uid, user_id, password;
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  string username;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          uid = value;
        }
      }
    }
  }
  std::ifstream filestream2(kPasswordPath);
  if (filestream2.is_open()) {
    while (std::getline(filestream2, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> value >> password >> user_id) {
        if (uid == user_id) {
          username = value;
        }
      }
    }
  }
  return username;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  long uptime;
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  std::stringstream ss(line);
  std::istream_iterator<std::string> begin(ss);
  std::istream_iterator<std::string> end;
  std::vector<std::string> vstrings(begin, end);
  if (vstrings.size() > 21) {
    uptime = stol(vstrings[21]);
  } else {
    return 0;
  }
  return uptime / sysconf(_SC_CLK_TCK);
}