# 命令工具

- [命令工具](#命令工具)
  - [帮助](#帮助)
  - [远程](#远程)
  - [文件](#文件)
    - [特殊文件](#特殊文件)
    - [df](#df)
    - [二进制分析](#二进制分析)
      - [nm](#nm)
      - [size](#size)
      - [ar](#ar)
      - [ld](#ld)
      - [ldconfig](#ldconfig)
      - [ldd](#ldd)
      - [readelf](#readelf)
      - [objdump](#objdump)
      - [hexdump](#hexdump)
      - [dumpbin](#dumpbin)
      - [strip](#strip)
  - [进程](#进程)
    - [top](#top)
    - [iostat](#iostat)
    - [iotop](#iotop)
    - [nohup](#nohup)
    - [ps](#ps)
    - [pstree](#pstree)
    - [strace](#strace)
    - [perf](#perf)
  - [网络](#网络)
    - [ethtool](#ethtool)
    - [ifconfig](#ifconfig)
    - [tcpdump](#tcpdump)

## 帮助

- `man [page]`
- `info [page]`

## 远程

- ssh 免密登录

```bash
#客户端生成公钥
ssh-keygen

#复制客户端id_rsa.pub内容，放置到服务器authorized_keys文件中
#~/.ssh/authorized_keys
```

## 文件

### 特殊文件

- 系统信息
  - `/proc/sys`目录可以查看或修改内核参数。
- 硬件信息
  - `/proc/cpuinfo`可以查看`CPU`信息。
  - `/proc/meminfo`可以查看内存信息。
  - `/proc/interrupts`统计所有的硬中断。
  - `/proc/softirqs`统计的所有的软中断信息。
  - `/proc/slabinfo`统计了内核数据结构的`slab`内存使用情况。
  - `/proc/net/dev`可以看到一些网卡统计数据。
  - `/sys/class/net/eth0/statistics/`也包含了网卡的统计信息。
- 进程信息
  - `/proc/[pid]/status`保存该`pid`进程的相关信息。
  - `/proc/[pid]/maps`保存该`pid`进程的虚拟地址空间信息。

### df

df(disk free)命令用于显示目前在 Linux 系统上的文件系统磁盘使用情况统计。

```bash
-h, --human-readable
  print sizes in powers of 1024 (e.g., 1023M)
--total
  elide all entries insignificant to available space, and produce a grand total
#常用
df --total -h
df . --total -h
```

### 二进制分析

#### nm

nm 是 names 的缩写，用来列出某些文件中的符号(函数和全局变量等)。

#### size

解析 ELF 文件的各段的大小。

#### ar

打包解包。

```bash
-t
  Display a table listing the contents of archive, or those of the files listed in member... that are present in the archive.  Normally only the member name is shown, but if the modifier O is specified, then the corresponding offset of the member is also displayed.  Finally, in order to see the modes (permissions), timestamp, owner, group, and size the v modifier should be included.
  If you do not specify a member, all files in the archive are listed.
  If there is more than one file with the same name (say, fie) in an archive (say b.a), ar t b.a fie lists only the first instance; to see them all, you must ask for a complete listing---in our example, ar t b.a.
-x
  Extract members (named member) from the archive.  You can use the v modifier with this operation, to request that ar list each name as it extracts it.
  If you do not specify a member, all files in the archive are extracted.
  Files cannot be extracted from a thin archive, and there are restrictions on extracting from archives created with P: The paths must not be absolute, may not contain "..", and any subdirectories in the paths must exist.  If it is desired to avoid these restrictions then used the --output option to specify an output directory.
```

#### ld

链接器。

```bash
-T scriptfile
--script=scriptfile
  Use scriptfile as the linker script.  This script replaces ld's default linker script (rather than adding to it), so commandfile must specify everything necessary to describe the output file. If scriptfile does not exist in the current directory, "ld" looks for it in the directories specified by any preceding -L options.  Multiple -T options accumulate.
-Bstatic
-dn
-non_shared
-static
  Do not link against shared libraries.  This is only meaningful on platforms for which shared libraries are supported.  The different variants of this option are for compatibility with various systems.  You may use this option multiple times on the command line: it affects library searching for -l options which follow it.  This option also implies --unresolved-symbols=report-all.  This option can be used with -shared.  Doing so means that a shared library is being created but that all of the library's external references must be resolved by pulling in entries from static libraries.
-e entry
--entry=entry
  Use entry as the explicit symbol for beginning execution of your program, rather than the default entry point.  If there is no symbol named entry, the linker will try to parse entry as a number, and use that as the entry address (the number will be interpreted in base 10; you may use a leading 0x for base 16, or a leading 0 for base 8).
-s
--strip-all
  Omit all symbol information from the output file.
```

#### ldconfig

ldconfig 主要是在默认搜寻目录/lib 和/usr/lib 以及动态库配置文件/etc/ld.so.conf 内所列的目录下，搜索出可共享的动态链接库，进而创建出动态装入程序(ld.so)所需的连接和缓存文件。
缓存文件默认为/etc/ld.so.cache，此文件保存已排好序的动态链接库名字列表，为了让动态链接库为系统所共享，需运行动态链接库的管理命令 ldconfig，此执行程序存放在/sbin 目录下。
ldconfig 通常在系统启动时运行，而当用户安装了一个新的动态链接库时，就需要手工运行这个命令。

#### ldd

查看依赖的动态库。

#### readelf

分析 ELF 文件。

```bash
-a
--all
  Equivalent to specifying --file-header, --program-headers, --sections, --symbols, --relocs, --dynamic, --notes, --version-info, --arch-specific, --unwind, --section-groups and --histogram.
  Note - this option does not enable --use-dynamic itself, so if that option is not present on the command line then dynamic symbols and dynamic relocs will not be displayed.
```

#### objdump

反汇编目标文件或者可执行文件。

```bash
-D
--disassemble-all
  Like -d, but disassemble the contents of all sections, not just those expected to contain instructions.
  This option also has a subtle effect on the disassembly of instructions in code sections.  When option -d is in effect objdump will assume that any symbols present in a code section occur on the boundary between instructions and it will refuse to disassemble across such a boundary.  When option -D is in effect however this assumption is supressed.  This means that it is possible for the output of -d and -D to differ if, for example, data is stored in code sections.
  If the target is an ARM architecture this switch also has the effect of forcing the disassembler to decode pieces of data found in code sections as if they were instructions.
  Note if the --dwarf=follow-links option has also been enabled then any symbol tables in linked debug info files will be read in and used when disassembling.
-t
--syms
  Print the symbol table entries of the file.  This is similar to the information provided by the nm program, although the display format is different.  The format of the output depends upon the format of the file being dumped, but there are two main types.  One looks like this:
    [  4](sec  3)(fl 0x00)(ty   0)(scl   3) (nx 1) 0x00000000 .bss
    [  6](sec  1)(fl 0x00)(ty   0)(scl   2) (nx 0) 0x00000000 fred
  where the number inside the square brackets is the number of the entry in the symbol table, the sec number is the section number, the fl value are the symbol's flag bits, the ty number is the symbol's type, the scl number is the symbol's storage class and the nx value is the number of auxilary entries associated with the symbol.  The last two fields are the symbol's value and its name.
  The other common output format, usually seen with ELF based files, looks like this:
    00000000 l    d  .bss   00000000 .bss
    00000000 g       .text  00000000 fred
  Here the first number is the symbol's value (sometimes refered to as its address).  The next field is actually a set of characters and spaces indicating the flag bits that are set on the symbol.  These characters are described below.  Next is the section with which the symbol is associated or *ABS* if the section is absolute (ie not connected with any section), or *UND* if the section is referenced in the file being dumped, but not defined there.
  After the section name comes another field, a number, which for common symbols is the alignment and for other symbol is the size.  Finally the symbol's name is displayed.
    "l"
    "g"
    "u"
    "!" The symbol is a local (l), global (g), unique global (u), neither global nor local (a space) or both global and local (!).  A symbol can be neither local or global for a variety of reasons, e.g., because it is used for debugging, but it is probably an indication of a bug if it is ever both local and global.  Unique global symbols are a GNU extension to the standard set of ELF symbol bindings.  For such a symbol the dynamic linker will make sure that in the entire process there is just one symbol with this name and type in use.
    "w" The symbol is weak (w) or strong (a space).
    "C" The symbol denotes a constructor (C) or an ordinary symbol (a space).
    "W" The symbol is a warning (W) or a normal symbol (a space).  A warning symbol's name is a message to be displayed if the symbol following the warning symbol is ever referenced.
    "I"
    "i" The symbol is an indirect reference to another symbol (I), a function to be evaluated during reloc processing (i) or a normal symbol (a space).
    "d"
    "D" The symbol is a debugging symbol (d) or a dynamic symbol (D) or a normal symbol (a space).
    "F"
    "f"
    "O" The symbol is the name of a function (F) or a file (f) or an object (O) or just a normal symbol (a space).
-T
--dynamic-syms
  Print the dynamic symbol table entries of the file.  This is only meaningful for dynamic objects, such as certain types of shared libraries.  This is similar to the information provided by the nm program when given the -D (--dynamic) option.
  The output format is similar to that produced by the --syms option, except that an extra field is inserted before the symbol's name, giving the version information associated with the symbol.  If the version is the default version to be used when resolving unversioned references to the symbol then it's displayed as is, otherwise it's put into parentheses.
```

#### hexdump

查看文件的[十六进制]编码。

```bash
 -C
  Canonical hex+ASCII display.  Display the input offset in hexadecimal, followed by sixteen space-separated, two column, hexadecimal bytes, followed by the same sixteen bytes in %_p format enclosed in ``|'' characters.
  Calling the command hd implies this option.
-n length
  Interpret only length bytes of input.
-s offset
  Skip offset bytes from the beginning of the input.  By default, offset is interpreted as a decimal number.  With a leading 0x or 0X, offset is interpreted as a hexadecimal number, otherwise, with a leading 0, offset is interpreted as an octal num‐ber.  Appending the character b, k, or m to offset causes it to be interpreted as a multiple of 512, 1024, or 1048576, respectively.
#常用
hexdump -C -s 0x1000 -n 100 a.out
```

#### dumpbin

- VC 分析目标文件和可执行文件工具。

```bash
/ALL 打印输出所有信息。
/SUMMARY 输出基本信息。
```

#### strip

去除符号信息。

```bash
-s
--strip-all
  Remove all symbols.
-g
-S
-d
--strip-debug
  Remove debugging symbols only.
--strip-unneeded
  Remove all symbols that are not needed for relocation processing.
```

## 进程

### top

```bash
load average
  system load avg over the last 1, 5 and 15 minutes
Tasks
  shows total tasks or threads, depending on the state of the Threads-mode toggle.  That total is further classified as: running; sleeping; stopped; zombie
%Cpu(s)
  shows CPU state percentages based on the interval since the last refresh.
  As  a  default,  percentages  for  these individual categories are displayed.  Where two labels are shown below, those for more recent kernel versions are shown first.
    us, user    : time running un-niced user processes
    sy, system  : time running kernel processes
    ni, nice    : time running niced user processes
    id, idle    : time spent in the kernel idle handler
    wa, IO-wait : time waiting for I/O completion
    hi : time spent servicing hardware interrupts
    si : time spent servicing software interrupts
    st : time stolen from this vm by the hypervisor
  1-id = us+sy+si
Mem
  This  portion  consists of two lines which may express values in kibibytes (KiB) through exbibytes (EiB) depending on the scaling factor enforced with the `E' interactive command.
  As a default, Line 1 reflects physical memory, classified as: total, free, used and buff/cache
  Line 2 reflects mostly virtual memory, classified as: total, free, used and avail (which is physical memory)
  The avail number on line 2 is an estimation of physical memory available for starting new applications, without swapping.  Unlike the free field,  it  at‐tempts  to  account for readily reclaimable page cache and memory slabs.  It is available on kernels 3.14, emulated on kernels 2.6.27+, otherwise the same as free.

#wa代表iowait，是 CPU 等待 IO 完成操作花费的时间占 CPU 的百分比。
#状态包括运行（R）、空闲（I）、不可中断睡眠（D）、可中断睡眠（S）、僵尸（Z）以及暂停（T）等。
#默认top命令配置显示的是平均的CPU使用情况，如果按下键盘1可以显示各颗逻辑CPU的使用情况。
#按 M，看内存占用。
#按 P，看 CPU 占用。
#按组合键 xb，然后用<>手动选择排序的列。
#按下字母f,进入列配置页面
```

### iostat

显示实际硬盘读写情况。

### iotop

查看进程实际占用 I/O。

### nohup

`nohup`(`no hang up`)，用于在系统后台不挂断地运行命令，退出终端不会影响程序的运行。在默认情况下，会输出一个名叫`nohup.out`的文件到当前目录下，如果当前目录的`nohup.out`文件不可写，输出重定向到`$HOME/nohup.out`文件中。

```bash
#常用
nohup application [arg …] [2>&1] [&]
```

### ps

`Linux ps`(`process status`)命令用于显示当前进程的状态，类似于`windows`的任务管理器。

```bash
#运行参数
-A 列出所有的进程
-e 同 -A
-f 显示详细信息
-w 显示加宽可以显示较多的信息
-au 显示较详细的信息
-aux 显示所有包含其他使用者的进程
  au(x) 输出格式 :
    USER PID %CPU %MEM VSZ RSS TTY STAT START TIME COMMAND
    USER: 进程拥有者
    PID: pid
    %CPU: 占用的 CPU 使用率
    %MEM: 占用的内存使用率
    VSZ: 占用的虚拟内存大小
    RSS: 占用的内存大小
    TTY: 终端的次要装置号码 (minor device number of tty)
    STAT: 该进程的状态:
      D: 无法中断的休眠状态 (通常 IO 的进程)
      R: 正在执行中
      S: 静止状态
      T: 暂停执行
      Z: 不存在但暂时无法消除
      W: 没有足够的内存分页可分配
      <: 高优先序的进程
      N: 低优先序的进程
      L: 有内存分页分配并锁在内存内 (实时系统或捱A I/O)
    START: 进程开始时间
    TIME: 执行的时间
    COMMAND: 所执行的指令

#常用
ps -ef | grep test
```

### pstree

`pstree`命令以树状图显示进程间的关系(`display a tree of processes`)。`ps`命令可以显示当前正在运行的那些进程的信息，但是对于它们之间的关系却显示得不够清晰。在`Linux`系统中，系统调用`fork`可以创建子进程，通过`shell`也可以创建子进程，`Linux`系统中进程之间的关系天生就是一棵树，树的根就是进程`PID`为`1`的`init`进程。

```bash
#常用
pstree pid
pstree -p pid
```

### strace

`strace`常用来跟踪进程执行时的系统调用和所接收的信号。

```bash
#参数
-p: 跟踪指定的进程
-f: 跟踪由fork子进程系统调用
-F: 尝试跟踪vfork子进程系统调吸入，与-f同时出现时, vfork不被跟踪
-o filename: 默认strace将结果输出到stdout。通过-o可以将输出写入到filename文件中
-ff: 常与-o选项一起使用，不同进程(子进程)产生的系统调用输出到filename.PID文件
-r: 打印每一个系统调用的相对时间
-t: 在输出中的每一行前加上时间信息。 -tt 时间确定到微秒级。还可以使用-ttt打印相对时间
-v: 输出所有系统调用。默认情况下，一些频繁调用的系统调用不会输出
-s: 指定每一行输出字符串的长度,默认是32。文件名一直全部输出
-c: 统计每种系统调用所执行的时间，调用次数，出错次数。
-e expr: 输出过滤器，通过表达式，可以过滤出掉你不想要输出

#常用
strace -p pid
```

### perf

- Linux 性能计数器是一个基于内核的子系统，它提供一个性能分析框架，比如硬件(CPU、PMU(Performance Monitoring Unit))功能和软件(软件计数器、tracepoint)功能。
- 通过 perf，应用程序可以利用 PMU、tracepoint 和内核中的计数器来进行性能统计。
- Perf 可以对程序进行函数级别的采样，从而了解程序的性能瓶颈在哪里。其基本原理是：每隔一个固定时间，就是 CPU 上产生一个中断，看当前是哪个进程、哪个函数，然后给对应的进程和函数加一个统计值，这样就知道 CPU 有多少时间在某个进程或某个函数上了。

```bash
#追踪记录保存到perf.data
perf record -a --call-graph dwarf [-p [pid]] [application]
# record：表示记录
# -F n：表示每秒n次
# -g：表示记录调用栈
# --call-graph dward：表示分析调用栈的关系
# sleep n：持续n秒
# -a：表示对所有CPU采样
# -p pid：表示分析指定的进程id
# -o file：指定输出文件

#分析
perf report -i perf.data
```

## 网络

### ethtool

<details>
<summary>ethtool</summary>

```bash
-i 显示网卡驱动的信息，如驱动的名称、版本等
-S 查看网卡收发包的统计情况
-g/-G 查看或者修改RingBuffer的大小
-l/-L 查看或者修改网卡队列数
-c/-C 查看或者修改硬中断合并策略
```

</details>

### ifconfig

<details>
<summary>ifconfig</summary>

```bash
RX packets：接收的总包数
RX bytes：接收的字节数
RX errors：表示总的收包的错误数量
RX dropped：数据包已经进入了Ring Buffer，但是由于其它原因导致的丢包
RX overruns：表示了fifo的overruns，这是由于Ring Buffer不足导致的丢包
```

</details>

### tcpdump

[tcpdump 原理](../code/tcpdump)
![tcpdump捕获接收包](https://github.com/gongluck/images/blob/main/network/linux/tcpdump/tcpdump_recv.png)
![tcpdump捕获发送包](https://github.com/gongluck/images/blob/main/network/linux/tcpdump/tcpdump_send.png)
![tcpdump](https://github.com/gongluck/images/blob/main/network/linux/tcpdump/tcpdump.png)

```bash
#抓包选项:
-c: 指定要抓取的包数量。
-i interface: 指定tcpdump需要监听的接口。若未指定该选项，将从系统接口列表中搜寻编号最小的已配置好的接口(不包括loopback接口，要抓取loopback接口使用tcpdump -i lo)，一旦找到第一个符合条件的接口，搜寻马上结束。可以使用'any'关键字表示所有网络接口。
-n: 对地址以数字方式显式，否则显式为主机名，也就是说-n选项不做主机名解析。
-nn: 除了-n的作用外，还把端口显示为数值，否则显示端口服务名。
-N: 不打印出host的域名部分。例如tcpdump将会打印'nic'而不是'nic.ddn.mil'。
-P: 指定要抓取的包是流入还是流出的包。可以给定的值为"in"、"out"和"inout"，默认为"inout"。
-s len: 设置tcpdump的数据包抓取长度为len，如果不设置默认将会是65535字节。对于要抓取的数据包较大时，长度设置不够可能会产生包截断，若出现包截断，输出行中会出现"[|proto]"的标志(proto实际会显示为协议名)。但是抓取len越长，包的处理时间越长，并且会减少tcpdump可缓存的数据包的数量，从而会导致数据包的丢失，所以在能抓取我们想要的包的前提下，抓取长度越小越好。

#输出选项:
-e: 输出的每行中都将包括数据链路层头部信息，例如源MAC和目标MAC。
-q: 快速打印输出。即打印很少的协议相关信息，从而输出行都比较简短。
-X: 输出包的头部数据，会以16进制和ASCII两种方式同时输出。
-XX: 输出包的头部数据，会以16进制和ASCII两种方式同时输出，更详细。
-v: 当分析和打印的时候，产生详细的输出。
-vv: 产生比-v更详细的输出。
-vvv: 产生比-vv更详细的输出。

#其他功能性选项:
-D: 列出可用于抓包的接口。将会列出接口的数值编号和接口名，它们都可以用于"-i"后。
-F: 从文件中读取抓包的表达式。若使用该选项，则命令行中给定的其他表达式都将失效。
-w: 将抓包数据输出到文件中而不是标准输出。可以同时配合"-G time"选项使得输出文件每time秒就自动切换到另一个文件。可通过"-r"选项载入这些文件以进行分析和打印。保存的文件可以用wireshark打开分析！使用ctrl+C停止tcpdump抓包！
-r: 从给定的数据包文件中读取数据。使用"-"表示从标准输入中读取。

#常用
tcpdump -D
tcpdump -c 10 -i eth0 -nn -XX -vvv
```
