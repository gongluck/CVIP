# 命令工具

- [命令工具](#命令工具)
  - [帮助](#帮助)
  - [循环执行](#循环执行)
  - [远程](#远程)
  - [文件](#文件)
    - [特殊文件](#特殊文件)
    - [监控文件内容](#监控文件内容)
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
  - [性能分析](#性能分析)
    - [ethtool](#ethtool)
    - [free](#free)
    - [ifconfig](#ifconfig)
    - [iostat](#iostat)
    - [iotop](#iotop)
    - [ip](#ip)
    - [mount](#mount)
    - [mpstat](#mpstat)
    - [netstat](#netstat)
    - [perf](#perf)
    - [pidstat](#pidstat)
    - [pmap](#pmap)
    - [ps](#ps)
    - [sar](#sar)
    - [ss](#ss)
    - [tcpdump](#tcpdump)
    - [time](#time)
    - [traceroute](#traceroute)
    - [top](#top)
    - [turbostat](#turbostat)
    - [vmstat](#vmstat)
  - [others](#others)
    - [lsof](#lsof)
    - [nohup](#nohup)
    - [pstree](#pstree)
    - [strace](#strace)

## 帮助

- `man [page]`
- `info [page]`

## 循环执行

```bash
while true ; do echo a ; echo b ; done
```

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

### 监控文件内容

```bash
watch -d cat [file]
```

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
-W
--wide
  Don't break output lines to fit into 80 columns. By default readelf breaks section header and segment listing lines for 64-bit ELF files, so that they fit into 80 columns. This option causes readelf to print each section header resp. each segment one a single line, which is far more readable on terminals wider than 80 columns.
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

## 性能分析

### ethtool

```bash
query or control network driver and hardware settings

  -i --driver
    Queries the specified network device for associated driver information.
  -k --show-features --show-offload
    Queries the specified network device for the state of protocol offload and other features.
```

### free

```bash
Display amount of free and used memory in the system

  -m, --mebi
    Display the amount of memory in mebibytes.
  -w, --wide
    Switch  to  the wide mode. The wide mode produces lines longer than 80 characters. In this mode buffers and cache are reported in two separate columns.
```

### ifconfig

```bash
configure a network interface
```

### iostat

```bash
Report Central Processing Unit (CPU) statistics and input/output statistics for devices and partitions.

  -s
    Display a short (narrow) version of the report that should fit in 80 characters wide screens.
  -t
    Print  the time for each report displayed. The timestamp format may depend on the value of the S_TIME_FORMAT environment variable (see below).
  -x
    Display extended statistics.
  -z
    Tell iostat to omit output for any devices for which there was no activity during the sample period.
```

### iotop

```bash
simple top-like I/O monitor
```

### ip

```bash
show / manipulate routing, network devices, interfaces and tunnels

  route
    routing table entry.
```

### mount

```bash
mount a filesystem

  link
    network device.
  monitor
    watch for netlink messages.
  route
    routing table entry.
```

### mpstat

```bash
Report processors related statistics.

  -A
    This option is equivalent to specifying -n -u -I ALL.  This option also implies specifying -N ALL -P ALL unless these options are explicitly set on the command line.

#%usr     用户时间，不包括%nice
#%nice    以nice设置的优先级运行的进程的用户时间
#%sys     系统时间（内核）
#%iowait  IO等待
#%irq     硬件中断CPU使用率
#%soft    软件中断CPU使用率
#%steal   用在服务其他租户上的时间
#%guest   用在客户虚拟机上的CPU时间
#%gnice   用在提升优先级的客户机上的CPU时间
#%idle    空闲
```

### netstat

```bash
Print network connections, routing tables, interface statistics, masquerade connections, and multicast memberships

  -a, --all
    Show both listening and non-listening sockets.  With the --interfaces option, show interfaces that are not up
  --interfaces, -i
    Display a table of all network interfaces.
  --route, -r
    Display the kernel routing tables. See the description in route(8) for details.  netstat -r and route -e produce the same output.
  --statistics, -s
    Display summary statistics for each protocol.
```

### perf

![常用perf子命令](https://github.com/gongluck/images/blob/main/linux/perf/perf_subcommand.png)

```bash
Performance analysis tools for Linux

  record
    Run a command and record its profile into perf.data
      -a, --all-cpus
        System-wide collection from all CPUs (default if no target is specified).
      -e, --event=
        Select the PMU event.
      -F, --freq=
        Profile at this frequency. Use max to use the currently maximum allowed frequency, i.e. the value in the kernel.perf_event_max_sample_rate sysctl. Will throttle down to the currently maximum allowed frequency. See --strict-freq.
      -g
        Enables call-graph (stack chain/backtrace) recording.
      --call-graph
        Setup and enable call-graph (stack chain/backtrace) recording, implies -g. Default is "fp" (for user space).
      -o, --output=
        Output file name.
      -p, --pid=
        Record events on existing process ID (comma separated list).

  report
    Read perf.data (created by perf record) and display the profile
      -i, --input=
        Input file name. (default: perf.data unless stdin is a fifo)

  list [*]
    List all symbolic event types

  stat
    Run a command and gather performance counter statistics

  trace
    This command will show the events associated with the target, initially syscalls, but other system events like pagefaults, task lifetime events, scheduling events, etc.
    This is a live mode tool in addition to working with perf.data files like the other perf tools. Files can be generated using the perf record command but the session needs to include the raw_syscalls events (-e raw_syscalls:*). Alternatively, perf trace record can be used as a shortcut to automatically include the raw_syscalls events when writing events to a file.
      -e, --expr, --event
        List of syscalls and other perf events (tracepoints, HW cache events, etc) to show. Globbing is supported, e.g.: "epoll_*", "msg", etc. See perf list for a complete list of events. Prefixing with ! shows all syscalls but the ones specified. You may need to escape it.
      --filter=<filter>
        Event filter. This option should follow an event selector (-e) which selects tracepoint event(s).

#追踪记录
perf record -F 99 -a -g [-p [pid]] [application] -o perf.data -- sleep 30
#分析
perf report -i perf.data
#火焰图
git clone https://github.com/brendangregg/FlameGraph
cd FlameGraph
# on-cpu
perf record -F 99 -a -g -o perf.data -- sleep 30
perf script -i perf.data | ./stackcollapse-perf.pl --all | ./flamegraph.pl > oncpu.svg
# off-cpu
perf record -F 99 -a -g -e sched:sched_switch,sched:sched_wakeup -o perf.data -- sleep 30
perf script -i perf.data | ./stackcollapse-perf.pl --all | ./flamegraph.pl > offcpu.svg
```

### pidstat

```bash
Report statistics for Linux tasks.

  -d
    Report I/O statistics (kernels 2.6.20 and later only).
  -p { pid[,...] | SELF | ALL }
    Select tasks (processes) for which statistics are to be reported.  pid is the process identification number. The SELF keyword indicates  that statistics are to be reported for the pidstat process itself, whereas the ALL keyword indicates that statistics are to be reported for all the tasks managed by the system.
  -t
    Also display statistics for threads associated with selected tasks.
    This option adds the following values to the reports:
      TGID   The identification number of the thread group leader.
      TID    The identification number of the thread being monitored.
```

### pmap

```bash
pmap [options] pid [...]
report memory map of a process

  -x, --extended
    Show the extended format.
  -X
    Show even more details than the -x option. WARNING: format changes according to /proc/PID/smaps
  -XX
    Show everything the kernel provides
```

### ps

```bash
report a snapshot of the current processes.

  -a
    Select all processes except both session leaders (see getsid(2)) and processes not associated with a terminal.
  -e
    Select all processes.  Identical to -A.
  -f
    Do full-format listing.  This option can be combined with many other UNIX-style options to add additional columns.  It also causes the command arguments to be printed.  When used with -L, the NLWP (number of threads) and LWP (thread ID) columns will be added.  See the c option, the format keyword args, and the format keyword comm.
  -u
    Display user-oriented format.
  -w
    Wide output.  Use this option twice for unlimited width.
  --width n
    Set screen width.
  -x
    Lift the BSD-style "must have a tty" restriction, which is imposed upon the set of all processes when some BSD-style (without "-") options are used or when the ps personality setting is BSD-like.  The set of processes selected in this manner is in addition to the set of processes selected by other means.  An alternate description is that this option causes ps to list all processes owned by you (same EUID as ps), or to list all processes when used together with the a option.

ps -ef
ps aux
```

### sar

```bash
Collect, report, or save system activity information.

  -A
    This is equivalent to specifying -bBdFHSvwWy -I SUM -m ALL -n ALL -q ALL -r ALL -u ALL. This option also implies specifying -I ALL -P ALL unless these options are explicitly set on the command line.
  -B
    Report paging statistics.
  -d
    Report activity for each block device.  When data are displayed, the device name is displayed as it (should) appear in /dev.  sar uses data in /sys to determine the device name based on its major and minor numbers.  If this name resolution fails, sar will use name  mapping  controlled  by /etc/sysstat/sysstat.ioconf file.  Persistent device names can also be printed if option -j is used (see below). Statistics for all devices are displayed unless a restricted list is specified using option --dev= (see  corresponding  option entry).  Note that disk activity depends on sadc options -S DISK and -S XDISK to be collected.
  -H
    Report hugepages utilization statistics.
  -n { keyword[,...] | ALL }
    Report network statistics.
    Possible keywords are DEV, EDEV, FC, ICMP, EICMP, ICMP6, EICMP6, IP, EIP, IP6, EIP6, NFS, NFSD, SOCK, SOCK6, SOFT, TCP, ETCP, UDP and UDP6.
    With the DEV keyword, statistics from the network devices are reported.  Statistics for all network interfaces are displayed  unless a restricted list is specified using option --iface= (see corresponding option entry).
  -q [ keyword[,...] | ALL ]
    Report system load and pressure-stall statistics.
    Possible keywords are CPU, IO, LOAD, MEM and PSI.
  -r [ ALL ]
    Report memory utilization statistics. The ALL keyword indicates that all the memory fields should be  displayed.
  -S
    Report swap space utilization statistics.
  -u [ ALL ]
    Report CPU utilization. The ALL keyword indicates that all the CPU fields should be displayed.
  -W
    Report swapping statistics.
```

### ss

```bash
another utility to investigate sockets

  -e, --extended
    Show detailed socket information. The output format is:
      uid:<uid_number> ino:<inode_number> sk:<cookie>
  -i, --info
    Show internal TCP information.
  -m, --memory
    Show socket memory usage. The output format is:
      skmem:(r<rmem_alloc>,rb<rcv_buf>,t<wmem_alloc>,tb<snd_buf>,f<fwd_alloc>,w<wmem_queued>,o<opt_mem>,bl<back_log>,d<sock_drop>)
  -p, --processes
    Show process using socket.
  -t, --tcp
    Display TCP sockets.
  -u, --udp
    Display UDP sockets.
```

### tcpdump

[tcpdump 原理](../code/tcpdump)
![tcpdump捕获接收包](https://github.com/gongluck/images/blob/main/network/linux/tcpdump/tcpdump_recv.png)
![tcpdump捕获发送包](https://github.com/gongluck/images/blob/main/network/linux/tcpdump/tcpdump_send.png)
![tcpdump](https://github.com/gongluck/images/blob/main/network/linux/tcpdump/tcpdump.png)

```bash
dump traffic on a network

  -c count
    Exit after receiving count packets.
  -e
    Print  the  link-level  header on each dump line.  This can be used, for example, to print MAC layer addresses for protocols such as Ethernet and IEEE 802.11.
  -i interface
  --interface=interface
    Listen  on interface.  If unspecified, tcpdump searches the system interface list for the lowest numbered, configured up interface (excluding loopback), which may turn out to be, for example, ''eth0''.
    On Linux systems with 2.2 or later kernels, an interface argument of ''any'' can be used to capture packets from all interfaces.   Note  that captures on the ''any'' device will not be done in promiscuous mode.
    If the -D flag is supported, an interface number as printed by that flag can be used as the interface argument, if no interface on the system has that number as a name.
  -n
    Don't convert addresses (i.e., host addresses, port numbers, etc.) to names.
  -N
    Don't print domain name qualification of host names.  E.g., if you give this flag then tcpdump will print ``nic'' instead of ``nic.ddn.mil''.
  -p
  --no-promiscuous-mode
    Don't put the interface into promiscuous mode.  Note that the interface might be in promiscuous mode for some other reason; hence, `-p'  can‐not be used as an abbreviation for `ether host {local-hw-addr} or ether broadcast'.
  -q
    Quick (quiet?) output.  Print less protocol information so output lines are shorter.
  -s snaplen
  --snapshot-length=snaplen
    Snarf  snaplen  bytes  of data from each packet rather than the default of 262144 bytes.  Packets truncated because of a limited snapshot are indicated in the output with ``[|proto]'', where proto is the name of the protocol level at which the truncation  has  occurred.   Note  that taking  larger  snapshots  both  increases  the  amount  of time it takes to process packets and, effectively, decreases the amount of packet buffering.  This may cause packets to be lost.  You should limit snaplen to the smallest number that will capture  the  protocol  information you're  interested in.  Setting snaplen to 0 sets it to the default of 262144, for backwards compatibility with recent older versions of tcp‐dump.
  -v
    When  parsing  and printing, produce (slightly more) verbose output.  For example, the time to live, identification, total length and options in an IP packet are printed.  Also enables additional packet integrity checks such as verifying the IP and ICMP header checksum.
    When writing to a file with the -w option, report, every 10 seconds, the number of packets captured.
  -vv
    Even more verbose output.  For example, additional fields are printed from NFS reply packets, and SMB packets are fully decoded.
  -vvv
    Even more verbose output.  For example, telnet SB ... SE options are printed in full.  With -X Telnet options are printed in hex as well.
  -x
    When parsing and printing, in addition to printing the headers of each packet, print the data of each packet (minus its link level header) in hex.   The smaller of the entire packet or snaplen bytes will be printed.  Note that this is the entire link-layer packet, so for link layers that pad (e.g. Ethernet), the padding bytes will also be printed when the higher layer packet is shorter than the required padding.
  -xx
    When parsing and printing, in addition to printing the headers of each packet, print the data  of  each  packet,  including  its  link  level header, in hex.
  -X
    When parsing and printing, in addition to printing the headers of each packet, print the data of each packet (minus its link level header) in hex and ASCII.  This is very handy for analysing new protocols.
  -XX
    When parsing and printing, in addition to printing the headers of each packet, print the data  of  each  packet,  including  its  link  level header, in hex and ASCII.

#常用
tcpdump -i eth0 -n -XX -vvv
```

### time

```bash
run programs and summarize system resource usage
```

### traceroute

```bash
print the route packets trace to network host
```

### top

```bash
display Linux processes

  -o  :Override-sort-field as:  -o fieldname
    Specifies the name of the field on which tasks will be sorted, independent of what is reflected in the configuration file.  You can prepend  a '+' or '-' to the field name to also override the sort direction.  A leading '+' will force sorting high to low, whereas a '-' will ensure a low to high ordering.
    This option exists primarily to support automated/scripted batch mode operation.

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
  This  portion  consists of two lines which may express values in kibibytes (KiB) through exbibytes (EiB) depending on the scaling factor enforced with the 'E' interactive command.
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

### turbostat

```bash
Report processor frequency and idle statistics
```

### vmstat

```bash
Report virtual memory statistics
  vmstat reports information about processes, memory, paging, block IO, traps, disks and cpu activity.
  The first report produced gives averages since the last reboot.  Additional reports give information on a sampling period of length delay.  The process and memory reports are instantaneous in either case.

#r      运行队列长度
#swpd   交换出的内存量
#free   空闲的可用内存
#buff   用于缓冲缓存的内存
#cache  用于页缓存的内存
#si     换入的内存（换页）
#so     换出的内存（换页）
#us     用户时间比例
#sy     系统（内核）时间比例
#id     空闲比例
#wa     等待IO比例
#st     窃取比例
```

## others

### lsof

查看进程打开的文件。

### nohup

no hang up，用于在系统后台不挂断地运行命令，退出终端不会影响程序的运行。在默认情况下，会输出一个名叫`nohup.out`的文件到当前目录下，如果当前目录的`nohup.out`文件不可写，输出重定向到`$HOME/nohup.out`文件中。

```bash
#常用
nohup COMMAND [arg …] [2>&1] [&] # 2>&1 将标准错误 2 重定向到标准输出 &1
```

### pstree

display a tree of processes，将所有进程以树状图显示，树状图将会以 pid (如果有指定) 或是以 init 这个基本进程为根 (root)，如果有指定使用者 id，则树状图会只显示该使用者所拥有的进程。

```bash
pstree [pid|user]
-p
  Show PIDs.  PIDs are shown as decimal numbers in parentheses after each process name.  -p implicitly disables compaction.
```

### strace

跟踪进程执行时的系统调用和所接收的信号。

```bash
-p pid
--attach=pid
  Attach to the process with the process ID pid and begin tracing.  The trace may be terminated at any time by a keyboard interrupt signal (CTRL-C).  strace will respond by detaching itself from the traced process(es) leaving it (them) to continue running.  Multiple  -p  op‐tions can be used to attach to many processes in addition to command (which is optional if at least one -p option is given).  -p "`pidof PROG`" syntax is supported.
-f
  Trace child processes as they are created by currently traced processes as a result of the fork(2), vfork(2) and clone(2) system  calls.Note that -p PID -f will attach all threads of process PID if it is multi-threaded, not only thread with thread_id = PID.
-ff
  If  the  -o  filename  option  is in effect, each processes trace is written to filename.pid where pid is the numeric process id of each process.  This is incompatible with -c, since no per-process counts are kept.
-r
  Print  a relative timestamp upon entry to each system call.  This records the time difference between the beginning of successive system calls.  Note that since -r option uses the monotonic clock time for measuring time difference and not the wall clock time, its  measure‐ments can differ from the difference in time reported by the -t option.
-s strsize
--string-limit=strsize
  Specify  the maximum string size to print (the default is 32).  Note that filenames are not considered strings and are always printed in full.
-t
  Prefix each line of the trace with the wall clock time.
-tt
  If given twice, the time printed will include the microseconds.
-ttt
  If given thrice, the time printed will include the microseconds and the leading portion will be printed as the number of  seconds  since the epoch.

#常用
strace -p [pid] -tt
```
