cmake_minimum_required(VERSION 2.8.3)
project(cpumem_monitor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Boost REQUIRED COMPONENTS
# regex
filesystem
system
)

include_directories(
  include
  ${YAMLCPP_INCLUDE_DIR}
)

add_executable(cpumem_monitor src/monitor_main.cpp src/robotinfo.cpp
			 				 src/cpu.cpp src/file_utils.cpp src/memory.cpp
			 				 src/lm_sensor.cpp src/lm_sensorlist.cpp)
target_link_libraries(cpumem_monitor
    ${Boost_LIBRARIES}
    ${catkin_LIBRARIES}
    ${YAMLCPP_LIBRARIES}
	pthread
    /usr/lib/x86_64-linux-gnu/libsensors.so
)
