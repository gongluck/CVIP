# BPF

- [BPF](#bpf)
  - [BPF/eBPF 原理](#bpfebpf-原理)
  - [检测类型](#检测类型)
  - [事件查询](#事件查询)
    - [文件系统](#文件系统)
    - [perf](#perf)
    - [bpftrace](#bpftrace)
  - [BPF 开发和执行](#bpf-开发和执行)
    - [编译 BPF 模块](#编译-bpf-模块)
    - [加载/卸载 BPF 模块](#加载卸载-bpf-模块)
    - [绑定/解绑 BPF 模块到事件](#绑定解绑-bpf-模块到事件)
    - [查询 BPF 模块](#查询-bpf-模块)
    - [追踪输出](#追踪输出)
  - [使用方案](#使用方案)
    - [bpf 系统调用](#bpf-系统调用)
    - [bpftrace](#bpftrace-1)
    - [BCC](#bcc)
    - [libbpf](#libbpf)
  - [BPF 运行时](#bpf-运行时)
  - [BPF 映射](#bpf-映射)
  - [BTF](#btf)
  - [bpftool](#bpftool)
  - [使用场景](#使用场景)
    - [XDP](#xdp)
    - [TC](#tc)
  - [内核源码里的 BPF 示例代码](#内核源码里的-bpf-示例代码)
    - [下载内核源码](#下载内核源码)
    - [编译 BPF 示例代码](#编译-bpf-示例代码)

## BPF/eBPF 原理

- BPF（Berkeley Packet Filter）和 eBPF（Extended Berkeley Packet Filter）的原理都基于在内核空间执行用户定义的代码，从而实现高效的数据包和系统事件处理。
- BPF 的原理是，在内核空间中定义一组过滤规则，通过执行用户定义的 BPF 代码，对网络数据包进行过滤和处理。BPF 代码通过一组特定的指令集，在内核中执行，可以快速地判断和处理数据包。
- eBPF 的原理是，在内核空间中定义一组更加灵活和强大的处理规则，通过执行用户定义的 eBPF 代码，对网络数据包和系统事件进行更加高级别的处理和分析。

## 检测类型

- 静态检测描述的是添加到源代码中的硬编码的软件检测点，有内核的 `tracepoint（跟踪点）`，还有针对用户空间软件的 `USDT（用户静态定义跟踪）`。
- 动态检测是在软件运行后，通过修改内存指令插入检测程序来创建检测点，有 `kprobes（内核探针）`、`kretprobes（内核返回探针）`、`uprobes（用户空间探针）`、`uretprobes（用户级返回探针）`。

## 事件查询

### 文件系统

```bash
# 所有导出符号
cat /proc/kallsyms
# 可用的 tracepoint 过滤函数的列表
cat /sys/kernel/debug/tracing/available_filter_functions
# 参数与格式
cat /sys/kernel/debug/tracing/events/.../format
```

### [perf](./command.md#perf)

```bash
# 查看支持的事件
perf list [*]
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

### 编译 BPF 模块

```bash
clang -target bpf -I /usr/include/$(uname -m)-linux-gnu -g -O2 -c [*.bpf.c] -o [*.bpf.o]
```

### 加载/卸载 BPF 模块

```bash
bpftool prog load [*.bpf.o] [/sys/fs/bpf/*]
rm [/sys/fs/bpf/*]
```

### 绑定/解绑 BPF 模块到事件

```bash
bpftool net attach xdp id [id] dev [dev]
bpftool net detach xdp dev [dev]
```

### 查询 BPF 模块

```bash
bpftool prog list [--pretty]
bpftool net list
bpftool prog show id [id] [--pretty]
bpftool prog dump xlated id [id]
bpftool prog dump jited id [id]
```

### 追踪输出

```bash
cat /sys/kernel/debug/tracing/trace_pipe
bpftool prog tracelog
```

## 使用方案

### bpf 系统调用

```c++
#include <linux/bpf.h>
int bpf(int cmd, union bpf_attr *attr, unsigned int size);
```

![bpf_syscall](https://github.com/gongluck/images/blob/main/linux/bpf/bpf_syscall.png)

- cmd，操作命令。
- attr，eBPF 属性指针，不同类型的操作命令需要传入不同的属性参数。
- size，代表属性的大小。
- **不同版本的内核所支持的 BPF 命令是不同的**，具体支持的命令列表可以参考内核头文件 `include/uapi/linux/bpf.h` 中 `bpf_cmd` 的定义。

### bpftrace

![bpftrace](https://github.com/gongluck/images/blob/main/linux/bpf/bpftrace.png)

- bpftrace 通常用在快速排查和定位系统上，功能有限不支持特别复杂的 eBPF 程序，也依赖于 BCC 和 LLVM 动态编译执行。
- bpftrace 中，函数参数可以使用内置变量 arg0..N。

### [BCC](https://github.com/iovisor/bcc.git)

```bash
# 二进制安装
apt-get install bpfcc-tools linux-headers-$(uname -r)
```

- [样例代码](../code/ebpf/bcc)
- BCC 是一个 BPF 编译器集合，依赖于 LLVM 和内核头文件，包含了用于构建 BPF 程序的编程框架和库，并提供了大量可以直接使用的工具。
- 用高级语言开发的 eBPF 程序，需要首先编译为 BPF 字节码，然后借助 bpf 系统调用加载到内核中，最后再通过性能监控等接口与具体的内核事件进行绑定。这样，内核的性能监控模块才会在内核事件发生时，自动执行 eBPF 程序。
- 在 BCC 中，与 eBPF 程序中 `BPF_PERF_OUTPUT` 相对应的用户态辅助函数是 `open_perf_buffer()` 。它需要传入一个回调函数，用于处理从 Perf 事件类型的 BPF 映射中读取到的数据。而后通过一个循环调用 `perf_buffer_poll` 读取映射的内容，并执行回调函数。

### libbpf

```bash
apt install make gcc libelf-dev pkg-config
# 编译源码
git clone https://github.com/libbpf/libbpf.git
cd libbpf/src/
make -j 8
make install DESTDIR=/
```

- [样例代码](../code/ebpf/libbpf)
- libbpf 是从内核中抽离出来的标准库，用它开发的 eBPF 程序可以直接分发执行，不需要每台机器都安装 LLVM 和内核头文件。
- libbpf 要求内核开启 BTF 特性，需要非常新的发行版才会默认开启（如 RHEL 8.2+ 和 Ubuntu 20.10+ 等）。
- 通过 SEC() 宏定义的数据结构和函数会放到特定的 ELF 段中，这样后续在加载 BPF 字节码时，就可以从这些段中获取所需的元数据。

## BPF 运行时

![BPF运行时](https://github.com/gongluck/images/blob/main/linux/bpf/bpf_runtime.png)
![BPF程序](https://github.com/gongluck/images/blob/main/linux/bpf/bpf_program.png)

- **eBPF 辅助函数** 提供了一系列用于 eBPF 程序与内核其他模块进行交互的函数。
- **eBPF 验证器** 确保 eBPF 程序的安全。
- **11 个（软件实现的） 64 位寄存器、一个程序计数器和一个 512 字节的栈组成的存储模块** 用于控制 eBPF 程序的执行。
- 寄存器 0 用于辅助函数的返回值，也用于 eBPF 程序的返回值。寄存器 10 总是保持指向 eBPF 堆栈帧的指针（并且 eBPF 程序不能修改它）。
- 当调用 eBPF 程序时，寄存器 1 始终保存传递给该程序的上下文参数。
- **即时编译器** 将 eBPF 字节码编译成本地机器指令，以便更高效地在内核中执行。
- **BPF 映射（map）** 用于提供可被用户空间程序访问的大块存储，进而控制 eBPF 程序的运行状态。

## BPF 映射

```c++
// 原生版 BPF Map 创建方式
union bpf_attr my_map_attr {
  .map_type = BPF_MAP_TYPE_ARRAY,
  .key_size = sizeof(int),
  .value_size = sizeof(int),
  .max_entries = 1024,
  .map_flags = BPF_F_NO_PREALLOC,
};
int fd = bpf(BPF_MAP_CREATE, &my_map_attr, sizeof(my_map_attr));

// 简化版 BPF Map 创建方式
struct bpf_map_def SEC("maps") my_bpf_map = {
  .type       = BPF_MAP_TYPE_ARRAY,
  .key_size   = sizeof(int),
  .value_size   = sizeof(int),
  .max_entries = 1024,
  .map_flags   = BPF_F_NO_PREALLOC,
};

// 查看 /tools/lib/bpf/bpf.h 定义的辅助函数

// 使用 bpftool 查看BPF Map信息
bpftool map list
bpftool map dump id [map id]
```

- BPF 映射用于提供大块的键值存储，可被用户空间程序访问，进而获取 eBPF 程序的运行状态。eBPF 程序最多可以访问 64 个不同的 BPF 映射，并且不同的 eBPF 程序也可以通过相同的 BPF 映射来共享它们的状态。
- BPF 系统调用中并没有删除映射的命令，因为 BPF 映射会在用户态程序关闭文件描述符的时候自动删除（即 close(fd)）。想在程序退出后还保留映射，就需要调用 `BPF_OBJ_PIN` 命令，将映射挂载到 `/sys/fs/bpf` 中。
- bpf_map_create 创建 BPF Map。
- bpf_map_lookup_elem(map, key)，通过 key 查询 BPF Map，得到对应 value。
- bpf_map_update_elem(map, key, value, options)，通过 key-value 更新 BPF Map，如果这个 key 不存在，也可以作为新的元素插入到 BPF Map 中去。
- bpf_map_get_next_key(map, lookup_key, next_key)，遍历 BPF Map。

## BTF

```bash
# 列出BTF信息
bpftool btf list
bpftool btf dump id [id]
bpftool btf dump file [*.o]
# 导出内核数据结构
bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h
# 导出BPF模块头文件
bpftool gen skeleton [*.o] > [*.skel.h]
```

- 从内核 5.2 开始，只要开启了 `CONFIG_DEBUG_INFO_BTF`，在编译内核时，内核数据结构的定义就会自动内嵌在内核二进制文件 `vmlinux` 中。还可以把这些数据结构的定义导出到一个头文件中（通常命名为 vmlinux.h）。
- 通过对 BPF 代码中的访问偏移量进行重写，解决了不同内核版本中数据结构偏移量不同的问题。
- 在 libbpf 中预定义不同内核版本中的数据结构的修改，解决了不同内核中数据结构不兼容的问题。
- BTF 是 BPF 版本的调试信息。

## bpftool

```bash
# 列出通过perf_event_open()挂载的BPF程序
bpftool perf
# 列出全部BPF程序
bpftool prog show
# 翻译BPF指令为汇编
bpftool prog dump xlated id [id] [linum/opcodes/visual]
# 显示JIT编译之后的机器码
bpftool prog dump jited id [id]
# 查询当前系统支持的辅助函数列表
bpftool feature probe
# 查询支持程序类型
bpftool feature probe | grep program_type
```

## 使用场景

### XDP

![xdp](https://github.com/gongluck/images/blob/main/linux/bpf/xdp.png)

```bash
# 编译 XDP 程序
clang -O2 -target bpf -c drop_world.c -o drop_world.o
# 加载 XDP 程序
ip link set dev lo xdp obj drop_world.o sec xdp verbose
# 显示程序列表
bpftool net list dev lo
ip link show dev lo
# 卸载 XDP 程序
ip link set dev lo xdp off
```

- [样例代码](../code/ebpf/xdp)
- 当前主流内核版本的 Linux 系统在加载 XDP BPF 程序时，会自动在 native 和 generic 这两种模式选择，完成加载后，可以使用 ip 命令行工具来查看选择的模式。

### TC

- [样例代码](../code/ebpf/tc)

```bash
# 编译TC程序
clang -O2 -target bpf -c drop_tcp.c -o drop_tcp.o
# 为目标网卡创建clsact
sudo tc qdisc add dev lo clsact
# 加载 TC 程序
sudo tc filter add dev lo egress bpf da obj drop_tcp.o sec tc verbose
# 卸载 TC 程序
sudo tc filter del dev lo egress bpf da obj drop_tcp.o sec tc verbose
# 查看
sudo tc filter show dev lo egress
```

## 内核源码里的 BPF 示例代码

### 下载内核源码

```bash
# 第一种方式
# 搜索
apt-cache search linux-source
  linux-source - Linux kernel source with Ubuntu patches
  linux-source-5.15.0 - Linux kernel source for version 5.15.0 with Ubuntu patches
  linux-source-5.19.0 - Linux kernel source for version 5.19.0 with Ubuntu patches
# 安装
apt install linux-source-5.15.0

# 第二种方式
apt-get source linux

# 以上两种方式，内核源代码均下载至/usr/src/目录下

# 在线查看代码
https://elixir.bootlin.com/linux/v5.15/source/samples/bpf
```

### 编译 BPF 示例代码

```bash
# 切换到内核源代码根目录
cd /usr/src/linux-source-5.15.0/
# 生成内核编译时需要的头文件
make headers_install
# 可视化选择你想为内核添加的内核模块，最终生成保存了相关模块信息的.config文件，为执行后面的命令做准备
make menuconfig
# 使用make命令编译samples/bpf/目录下所有bpf示例代码，注意需要加上最后的/符号
make samples/bpf/ # or  make M=samples/bpf

# 编译bpftool
make -C bpf/bpftool/

# 添加自定义BPF代码
# 追加新的一行至hostprogs-y开头的代码块最后，保证自己的BPF程序能够生成可执行文件
hostprogs-y += my_bpf
# 一般BPF程序使用以下命令即可，具体取决于你的程序是否依赖其他特殊头文件
my_bpf-objs := my_bpf_user.o
# 3. 追加新的一行至always开头的代码块最后，保证触发生成可执行文件的任务
always += my_bpf_kern.o
```
