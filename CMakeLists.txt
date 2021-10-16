cmake_minimum_required(VERSION 3.0)
project(pkt_switch)

find_package(SystemCLanguage CONFIG REQUIRED)
set (CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD})

add_executable (pkt_switch main.cpp
                           pkt.h
                           fifo.h
                           fifo.cpp                   
                           switch_clk.h
                           switch_clk.cpp
                           switch_reg.h
                           switch.h
                           switch.cpp
                           host.h
                           host.cpp
                           spine.h
                           spine.cpp)
target_link_libraries(pkt_switch SystemC::systemc)