# GCC

- [GCC](#gcc)
  - [gcc](#gcc-1)
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
