# 利用Cmake 构建 SYSYTEMC 工程
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



