cmake_minimum_required(VERSION 3.0)
project(pkt_switch) ##指定项目名称 

find_package(SystemCLanguage CONFIG REQUIRED)## 指定三方包
set (CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD})## 设置语言标准

add_executable (pkt_switch main.cpp
                           pkt.h
                           fifo.h
                           fifo.cpp                   
                           switch_clk.h
                           switch_clk.cpp
                          
                           host.h
                           host.cpp
                           spine.h
                           spine.cpp
                           leaf.h
                           leaf.cpp
                           )## 添加源码文件 以后要是有添加文件直接修改这块就行了
target_link_libraries(pkt_switch SystemC::systemc)