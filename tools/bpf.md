# BPF

- [BPF](#bpf)
  - [BPF/eBPF 原理](#bpfebpf-原理)
  - [检测技术](#检测技术)
  - [环境依赖](#环境依赖)
  - [事件查询](#事件查询)
    - [perf](#perf)
    - [bpftrace](#bpftrace)
  - [BPF 开发和执行](#bpf-开发和执行)
  - [使用方案](#使用方案)
    - [bpf 系统调用](#bpf-系统调用)
    - [bpftrace](#bpftrace-1)
    - [BCC](#bcc)
    - [libbpf](#libbpf)
  - [BPF 运行时](#bpf-运行时)
  - [BPF 映射](#bpf-映射)
  - [BTF](#btf)
  - [bpftool](#bpftool)
  - [faddr2line](#faddr2line)

## BPF/eBPF 原理

- BPF（Berkeley Packet Filter）和 eBPF（Extended Berkeley Packet Filter）的原理都基于在内核空间执行用户定义的代码，从而实现高效的数据包和系统事件处理。
- BPF 的原理是，在内核空间中定义一组过滤规则，通过执行用户定义的 BPF 代码，对网络数据包进行过滤和处理。BPF 代码通过一组特定的指令集，在内核中执行，可以快速地判断和处理数据包。
- eBPF 的原理是，在内核空间中定义一组更加灵活和强大的处理规则，通过执行用户定义的 eBPF 代码，对网络数据包和系统事件进行更加高级别的处理和分析。eBPF 还提供了一些新的工具和 API，可以更加方便地使用和管理 eBPF 程序。eBPF 的指令集比 BPF 更加丰富和灵活，可以实现更多种类的数据处理和分析。

## 检测技术

- 静态检测描述的是添加到源代码中的硬编码的软件检测点，有内核的 `tracepoint（跟踪点）`，还有针对用户空间软件的 `USDT（用户静态定义跟踪）`。
- 动态检测是在软件运行后，通过修改内存指令插入检测程序来创建检测点，有 `kprobes（内核探针）`、`kretprobes（内核返回探针）`、`uprobes（用户空间探针）`、`uretprobes（用户级返回探针）`。

## 环境依赖

```bash
apt install -y make clang llvm libelf-dev libbpf-dev bpfcc-tools libbpfcc-dev linux-tools-$(uname -r) linux-headers-$(uname -r)

#从内核源码编译
uname -r
#5.15.0-60-generic
apt-cache search linux-source
apt install linux-source-5.15.0
cd /usr/src/
tar -jxvf linux-source-5.15.0.tar.bz2
apt -y install binutils-dev libelf-dev libcap-dev
cd linux-source-5.15.0/tools
make -C bpf/bpftool
cd bpf/bpftool
make clean
make
make install
./bpftool version -p
```

## 事件查询

### [perf](./command.md#perf)

```bash
# 查看支持的事件
perf list [*]
# 查看参数与格式
cat /sys/kernel/debug/tracing/events/.../format
```

### bpftrace

```bash
# 查看支持的事件
bpftrace -lv [tracepoint/kprobe/kretprobe/usdt/uprobe/uretprobe/*:...:...]
```

## BPF 开发和执行

- eBPF 内部的内存空间只有寄存器和栈，而且不能随意调用内核函数，内核定义了一系列的辅助函数（`bpf_probe_read`等），用于 eBPF 程序与内核其他模块进行交互。
- 在 eBPF 程序需要大块存储时，必须通过 BPF 映射（BPF Map）来完成。
- 把 eBPF 程序加载到内核之后，还需要把加载后的程序跟具体的内核函数调用事件进行绑定。在 eBPF 的实现中，事件绑定都是通过 `perf_event_open()` 来完成的。
- 内核把所有函数以及非栈变量的地址都抽取到了 `/proc/kallsyms` 中，这样调试器就可以根据地址找出对应的函数和变量名称。
- 内核调试文件系统向用户空间提供了内核调试所需的基本信息，如内核符号列表、跟踪点、函数跟踪（ftrace）状态以及参数格式等。在终端中执行 `ls -lh /sys/kernel/debug` 来查询内核调试文件系统的具体信息。可以从 `/sys/kernel/debug/tracing` 中找到所有内核预定义的跟踪点，进而可以在需要时把 eBPF 程序挂载到对应的跟踪点。
- 在内核插桩和跟踪点两者都可用的情况下，应该选择更稳定的跟踪点，以保证 eBPF 程序的可移植性。

## 使用方案

### bpf 系统调用

```c++
#include <linux/bpf.h>
int bpf(int cmd, union bpf_attr *attr, unsigned int size);
```

- cmd，操作命令。
- attr，eBPF 属性指针，不同类型的操作命令需要传入不同的属性参数。
- size，代表属性的大小。
- **不同版本的内核所支持的 BPF 命令是不同的**，具体支持的命令列表可以参考内核头文件 `include/uapi/linux/bpf.h` 中 `bpf_cmd` 的定义。

### bpftrace

![bpftrace](https://github.com/gongluck/images/blob/main/linux/bpf/bpftrace.png)

- bpftrace 通常用在快速排查和定位系统上，功能有限不支持特别复杂的 eBPF 程序，也依赖于 BCC 和 LLVM 动态编译执行。
- bpftrace 中，函数参数可以使用内置变量 arg0..N。

### BCC

- BCC 是一个 BPF 编译器集合，依赖于 LLVM 和内核头文件，包含了用于构建 BPF 程序的编程框架和库，并提供了大量可以直接使用的工具。
- 用高级语言开发的 eBPF 程序，需要首先编译为 BPF 字节码，然后借助 bpf 系统调用加载到内核中，最后再通过性能监控等接口与具体的内核事件进行绑定。这样，内核的性能监控模块才会在内核事件发生时，自动执行 eBPF 程序。
- 在 BCC 中，与 eBPF 程序中 `BPF_PERF_OUTPUT` 相对应的用户态辅助函数是 `open_perf_buffer()` 。它需要传入一个回调函数，用于处理从 Perf 事件类型的 BPF 映射中读取到的数据。而后通过一个循环调用 `perf_buffer_poll` 读取映射的内容，并执行回调函数。

### libbpf

- libbpf 是从内核中抽离出来的标准库，用它开发的 eBPF 程序可以直接分发执行，不需要每台机器都安装 LLVM 和内核头文件。
- libbpf 要求内核开启 BTF 特性，需要非常新的发行版才会默认开启（如 RHEL 8.2+ 和 Ubuntu 20.10+ 等）。
- 通过 SEC() 宏定义的数据结构和函数会放到特定的 ELF 段中，这样后续在加载 BPF 字节码时，就可以从这些段中获取所需的元数据。
- 生成脚手架头文件：
  ```bash
  clang -target bpf -D__TARGET_ARCH_x86_64 -I/usr/include/x86_64-linux-gnu -c [*.bpf.c] -o [*.bpf.o]
  bpftool gen skeleton [*.bpf.o] > [*.skel.h]
  ```

## BPF 运行时

![BPF运行时](https://github.com/gongluck/images/blob/main/linux/bpf/bpf_runtime.png)
![BPF程序](https://github.com/gongluck/images/blob/main/linux/bpf/bpf_program.png)

- **eBPF 辅助函数** 提供了一系列用于 eBPF 程序与内核其他模块进行交互的函数。
- **eBPF 验证器** 确保 eBPF 程序的安全。
- **11 个 64 位寄存器、一个程序计数器和一个 512 字节的栈组成的存储模块** 用于控制 eBPF 程序的执行。
- **即时编译器** 将 eBPF 字节码编译成本地机器指令，以便更高效地在内核中执行。
- **BPF 映射（map）** 用于提供可被用户空间程序访问的大块存储，进而控制 eBPF 程序的运行状态。

## BPF 映射

- BPF 映射用于提供大块的键值存储，可被用户空间程序访问，进而获取 eBPF 程序的运行状态。eBPF 程序最多可以访问 64 个不同的 BPF 映射，并且不同的 eBPF 程序也可以通过相同的 BPF 映射来共享它们的状态。
- BPF 系统调用中并没有删除映射的命令，因为 BPF 映射会在用户态程序关闭文件描述符的时候自动删除（即 close(fd)）。想在程序退出后还保留映射，就需要调用 `BPF_OBJ_PIN` 命令，将映射挂载到 `/sys/fs/bpf` 中。

## BTF

- 从内核 5.2 开始，只要开启了 `CONFIG_DEBUG_INFO_BTF`，在编译内核时，内核数据结构的定义就会自动内嵌在内核二进制文件 `vmlinux` 中。还可以把这些数据结构的定义导出到一个头文件中（通常命名为 vmlinux.h）:
  ```bash
  bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h
  ```
- 通过对 BPF 代码中的访问偏移量进行重写，解决了不同内核版本中数据结构偏移量不同的问题。
- 在 libbpf 中预定义不同内核版本中的数据结构的修改，解决了不同内核中数据结构不兼容的问题。

## bpftool

```bash
# 列出eBPF程序
bpftool prog list
# 查看指令
bpftool prog dump xlated id [id]
# 查询当前系统支持的辅助函数列表
bpftool feature probe
# 查询支持程序类型
bpftool feature probe | grep program_type
```

## [faddr2line](https://github.com/torvalds/linux/blob/master/scripts/faddr2line)

```bash
# 解析堆栈代码行
faddr2line [source] [func+num]
```
