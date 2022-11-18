# 调试

- [调试](#调试)
  - [Visual Studio C++](#visual-studio-c)
  - [GCC(GDB)](#gccgdb)

## Visual Studio C++

  - `_CrtSetDbgFlag`函数允许应用程序来控制调试堆管理器通过修改的位域跟踪内存分配的方式`_crtDbgFlag`标志。
    |           标志            |                                                                           说明                                                                           |
    | :-----------------------: | :------------------------------------------------------------------------------------------------------------------------------------------------------: |
    |   _CRTDBG_ALLOC_MEM_DF    |                      ON:启用调试堆分配并使用内存块类型标识符<br>OFF:将新的分配添加到堆的链接列表,但设置阻止类型设置为_IGNORE_BLOCK                       |
    |  _CRTDBG_CHECK_ALWAYS_DF  |                                    ON:调用_CrtCheckMemory在每次分配和解除分配请求<br>OFF:必须显式调用_CrtCheckMemory                                     |
    |   _CRTDBG_CHECK_CRT_DF    |                            ON:包括泄漏检测和内存状态差异操作中的_CRT_BLOCK类型<br>OFF:这些操作将忽略运行时库在内部使用的内存                             |
    | _CRTDBG_DELAY_FREE_MEM_DF |             ON:将已释放的内存块在堆链接列表中,将它们分配_FREE_BLOCK键入,然后使用字节值0xDD填充它们<br>OFF:不要将已释放的块保留在堆链接列表中             |
    |   _CRTDBG_LEAK_CHECK_DF   | ON:执行自动泄露检查在程序退出时通过调用_CrtDumpMemoryLeaks,则生成错误报告,如果应用程序未能释放其所分配的所有内存<br>OFF:不要在程序退出时自动执行泄露检查 |
  - `_CrtDumpMemoryLeaks`显示内存泄漏的报告信息。
  - `_malloc_dbg`在具有额外空间的堆中为调试标头和覆盖缓冲区分配内存块。
    ```C++
    #if defined(_WIN32)
      #ifdef _DEBUG
        #define _CRTDBG_MAP_ALLOC
        #include <stdlib.h>
        #include <crtdbg.h>
        #define new new( _CLIENT_BLOCK, __FILE__, __LINE__)
      #endif
    #endif
    ```
  - `_CrtSetBreakAlloc`在指定的对象分配序号上设置断点。
  - `_CrtCheckMemory`确定分配在调试堆得内存块的完整性。
  - `SetUnhandledExceptionFilter`捕获程序未处理的异常。
  - `MiniDumpWriteDump`生成程序快照。

## [GCC(GDB)](./gcc.md#gdb)

  - `-fsanitize=address`编译参数，报告内存泄漏信息。
  - `gcore dumpfilepath` `gdb`命令，生成程序快照。调试快照使用`gdb exepath dumpfilepath`。
  - 设置系统生成`core dump`
    - `/etc/security/limits.conf`添加`soft core unlimited`。
    - `/proc/sys/kernel/core_pattern`添加`/root/corefile/core-%e-%s-%p-%t`。
  - `objcopy --only-keep-debug debugexe exe.symbol`生成调试符号。
  - `gdb --symbol=exe.symbol -exec=exefilepath`使用调试符号调试发行版。
  - `gdb --symbol=debugexe -exec=exefilepath`使用调试版程序调试发行版。
  - `gdb`设置动态库符号路径`set solib-search-path debugsopath`。
  