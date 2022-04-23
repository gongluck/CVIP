# GCC

- [GCC](#gcc)
  - [gcc](#gcc-1)
    - [fPIC](#fpic)
    - [Linux程序运行时查找动态库](#linux程序运行时查找动态库)
  - [gdb](#gdb)
    - [运行](#运行)
    - [源代码](#源代码)
    - [断点](#断点)
    - [栈帧](#栈帧)
    - [查看信息](#查看信息)

## gcc

  ```gcc
  gcc [-o outname] source [...] [-lstdc++] [-g(生成调试符号)]
  ```

### fPIC

  - ```fPIC```的全称是```Position Independent Code```,用于生成位置无关代码。
  - 不加```fPIC```编译的```so```必须要在加载到用户程序的地址空间时重定向所有表目,所以在它里面不能引用其它地方的代码。不加```-fPIC```生成的动态库,生成动态库时假定它被加载在地址```0```处。加载时它会被加载到一个地址```base```,这时要进行一次重定位```relocation```,把代码、数据段中所有的地址加上这个```base```的值。这样代码运行时就能使用正确的地址了。
  - 加上```fPIC```选项生成的动态库,显然是位置无关的。这样的代码本身就能被放到线性地址空间的任意位置,无需修改就能正确执行。通常的方法是获取指令指针的值,加上一个偏移得到全局变量/函数的地址。加```fPIC```选项的源文件对于它引用的函数头文件编写有很宽松的尺度。比如只需要包含个声明的函数的头文件,即使没有相应的```C```文件来实现,编译成```so```库照样可以通过。

### Linux程序运行时查找动态库

  - ```-Wl,rpath```
    - ```-Wl```表示编译器将后面的参数传递给链接器```ld```
    - ```-rpath```添加一个文件夹作为运行时库的搜索路径。在将```ELF```可执行文件与共享对象链接时使用此选项;在链接时,一些动态库明确的链接了其他动态库,则```-rpath```选项也可用于定位这些链接的动态库;在运行链接时,会优先搜索```-rpath```的路径,再去搜索```LD_RUN_PATH```的路径
  - ```/usr/lib```、```/lib```、```/usr/local/lib```
  - ```LD_LIBRARY_PATH```环境变量,只在当前```shell```有效
    ```shell
    export LD_LIBRARY_PATH=$mypath:$LD_LIBRARY_PATH
    ```
  - ```/etc/ld.so.conf```

## gdb

### 运行

  ```gdb
  # 选择调试模块
  gdb application
  # 附件进程调试
  gdb attach pid
  # 输入运行参数
  set args ...
  运行run
  r
  # 继续continue
  c [n]
  # 跳出函数
  finish
  # 单步执行step
  s
  # 逐过程执行next
  n [n]
  # 显示当前函数和代码行
  where
  # 跳转执行
  jump lineno
  jump funcname
  # 退出quit
  q
  ```

### 源代码

  ```gdb
  # 查看源码list
  l [-] [n]
  l funcname
  l filename:lineno
  # 设置显示行数
  set listsize n
  # 添加源代码搜索目录
  dir path
  ```
### 断点

  ```gdb
  # 设置断点break/临时断点tb
  b/tb source:lineno [if condition]
  b [namespace::]funcname[(parameterlist)]
  b +/- lineno
  b *address
  rb regular_expression
  # 查看断点info
  i b [breakno]
  # 启用/禁用断点
  enable/disable [once] [count n] breakno[-breakno]
  # 忽略断点前n次
  ignore breakno n
  # 删除断点
  delete [breakno[-breakno] ...]
  clear funcname
  clear source:lineno
  # 线程断点
  b [...] thread threadno
  ```

### 栈帧

  ```gdb
  # 调用栈backtrace
  bt [n]
  # 切换栈帧
  f[rame] frameno
  # 显示栈帧信息
  i f [frameno]
  # 当前栈帧参数
  i args
  # 当前栈帧局部变量
  i locals
  # 查看所有线程的回溯信息
  thread apply all bt
  ```

### 查看信息

  ```gdb
  # 观察点
  watch variable/expression
  # 读取观察点
  rwatch variable/expression
  # 读写观察点
  awatch variable/expression
  # 查看所有观察点
  i watchpoints
  # 启用/禁用/删除观察点
  enable/disable/delete watchno
  
  # 监视变量
  display variable
  info display
  enable/disable display displayno
  delete display [displayno]
  
  # 查看变量print
  p variable
  # 查看变量类型
  ptype /o variable

  # 显示优化
  set print pretty
  set print array on/off

  # 查看内存
  x [/s /d /x] address
  # 获取函数地址
  p funcname

  # 寄存器
  i r[egisters]
  i all-registers

  # 查看所有线程
  i threads
  # 切换线程
  thread threadno
  ```
