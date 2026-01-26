#linux-sysmon/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(linux-sysmon VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(sysmon src/main.cpp)

target_compile_options(sysmon PRIVATE -Wall -Wextra -O2)
