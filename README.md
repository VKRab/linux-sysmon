# Linux System Monitor (CLI)

Linux System Monitor is a lightweight **command-line system monitoring tool** written in **C++** for Linux systems.  
It provides real-time information about system resources directly in the terminal.

---

## Features

- CPU load (percentage)
- RAM usage (used / total)
- Disk usage for root filesystem (`/`)
- System uptime
- List of active processes (PID, name, state)
- Refresh every 2 seconds

---

## Example Output
```
Linux System Monitor (CLI)

CPU Load: 4.3%
RAM: 15228 MB / 15713 MB (96.9%)
Disk (/): 482923 MB / 707609 MB (68.2%)
Uptime: 07h 36m

Top Active Processes (PID | Name | State):
1 | systemd | S
2 | kthreadd | S
3 | pool_workqueue_relea | S
4 | kworker/R-rcu_gp | I
5 | kworker/R-sync_wq | I
6 | kworker/R-kvfree_rcu | I
7 | kworker/R-slub_flush | I
8 | kworker/R-netns | I
10 | kworker/0:0H-events_ | I

Updates every 2 seconds. Press Ctrl+C to exit
```

---

## Requirements

- Linux OS
- GCC / Clang with C++17 support
- CMake

---

## Build Instructions

Clone the repository:
```bash
git clone https://github.com/VKRab/linux-sysmon.git
cd linux-sysmon
```

Create a build directory:
```bash
mkdir build
cd build
cmake ..
make
```
After successful build, the binary sysmon will be created in the build directory.

## Installation Run as a Global Command

To run the application from any directory using a single command (sysmon), copy the binary to /usr/local/bin.
```bash
sudo cp sysmon /usr/local/bin/sysmon
sudo chmod +x /usr/local/bin/sysmon
```
Verify installation:
```bash
which sysmon
```
Expected output:
```text
/usr/local/bin/sysmon
```
Now you can start the system monitor simply by running:
```bash
sysmon
```

---

## Usage

-The screen updates every 2 seconds

-Press Ctrl + C to exit

---

## Project Goals
This project was created to demonstrate:

-Practical C++ development on Linux

-Working with Linux system interfaces (/proc, sysinfo, statvfs)

-Building CLI tools

-Basic process and resource monitoring
