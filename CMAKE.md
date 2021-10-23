# 利用Cmake 构建 SYSYTEMC 工程
## cmake 介绍
允许开发者编写一种平台无关的 CMakeList.txt 文件来定制整个编译流程，然后再根据目标用户的平台进一步生成所需的本地化 Makefile 和工程文件，如 Unix 的 Makefile 或 Windows 的 Visual Studio 工程。从而做到“Write once, run everywhere”。显然，CMake 是一个比上述几种 make 更高级的编译配置工具。一些使用 CMake 作为项目架构系统的知名开源项目有 VTK、ITK、KDE、OpenCV、OSG 等 。
## 安装CMake
### Linux下安装
首先输入cmake -version查看下系统安装了没有，没有的话继续安装，输入sudo apt install cmake安装。之后输入cmake -version看到提示信息后，说明安装成功。（ps 建议版本为9.3以上）
## build Systemc
1. 下载 SystemC 2.3.3，解压并进入目录systemc-2.3.3
    >cd /path/to/systemc-2.3.2
2. 创建 build directory
    >mkdir build
3. build Systemc with C++11 support ,初学时使用DEbug 模式，熟练后使用Release模式
    * debug模式
    ```
    cmake ../ -DCMAKE_CXX_STANDARD=11-DCMAKE_BUILD_TYPE=Debug
    ```
    * release 模式
    ```
    cmake ../ -DCMAKE_CXX_STANDARD=11 -DCMAKE_BUILD_TYPE=Release
    ```
1. 进入build 文件夹 并build
    >cmake --build .

## 创建一个Systemc project 
### main.cpp
``` C++
#include <systemc.h>

SC_MODULE (hello_world) {
    SC_CTOR (hello_world)
    {
        SC_THREAD(say_hello);
    }

    void say_hello()
    {
        cout << "Hello World SystemC" << endl;
    }

};

int sc_main(int argc, char* argv[])
{
    hello_world hello("HELLO");
    sc_start();

    return (0);
}
```

### CmakeLists.txt
```cmake_minimum_required(VERSION 3.0)
project(test_systemc)#指定工程名

find_package(SystemCLanguage CONFIG REQUIRED)
set (CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD})

add_executable(test_systemc main.cpp)#添加编译文件
target_link_libraries(test_systemc SystemC::systemc)#链接systemc库
```
### 创建build目录
>mkdir build

>cd build
### make
>cmake ..
### 执行可执行程序
>./test_systemc 
### 预期输出
```SystemC 2.3.2 --- Oct 14 2017 19:38:30
        Copyright (c) 1996-2017 by all Contributors,
        ALL RIGHTS RESERVED
Hello World SystemC
```
## 结合VSCODE的图形化操作构建PKTswitch 例子
1. 在vscode拓展搜索cmake拓展并安装
2. 将PKTswitch 包复制到虚拟机任一目录下
3. 在对应目录下打开vscode
4. 看vscode左边栏是否出现CMake选项，大概率在扩展选项下面。如没有出现请依次检查以下项目
   * 当前目录下的根目录是否有cmakelists文件。要注意是根目录。
   * 是否正确安装cmake拓展 和cmake
5. 如一切顺利vscode下方状态栏会出现如下项目
![状态栏 图标](pics/1.jpg)
6. 将以下内容复制粘贴到CMakelists.txt文件中，覆盖掉原先代码
```
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
                           switch_reg.h
                           switch.h
                           switch.cpp
                           host.h
                           host.cpp
                           spine.h
                           spine.cpp
                           match_functions.h
                           match_functions.cpp
                           )## 添加源码文件 以后要是有添加文件直接修改这块就行了
target_link_libraries(pkt_switch SystemC::systemc)
 ```
选择gcc版本为9.3及以上，点击build按钮即可完成构建





