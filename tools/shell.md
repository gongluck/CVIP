# Shell命令工具

- [Shell命令工具](#shell命令工具)
  - [文件](#文件)
    - [特殊文件](#特殊文件)
    - [df](#df)
    - [nm](#nm)
    - [size](#size)
    - [readelf](#readelf)
    - [objdump](#objdump)
    - [hexdump](#hexdump)
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

## 文件

### 特殊文件

- ```/proc/pid/status```保存该```pid```进程的相关信息
- ```/proc/sys```目录可以查看或修改内核参数
- ```/proc/cpuinfo```可以查看```CPU```信息
- ```/proc/meminfo```可以查看内存信息
- ```/proc/interrupts```统计所有的硬中断
- ```/proc/softirqs```统计的所有的软中断信息
- ```/proc/slabinfo```统计了内核数据结构的```slab```内存使用情况
- ```/proc/net/dev```可以看到一些网卡统计数据
- ```/sys/class/net/eth0/statistics/```也包含了网卡的统计信息

### df

```df```(```disk free```)命令用于显示目前在```Linux```系统上的文件系统磁盘使用情况统计。

```shell
#常用
df --total -h
df . --total -h
```

### nm

nm是names的缩写，用来列出某些文件中的符号(函数和全局变量等)。

### size

- 解析ELF文件的各段的大小。

### readelf

- 分析ELF文件。

  ```shell
  -h 显示elf文件开始的文件头信息。
  -S 显示段表信息。
  -s 显示符号表信息。
  ```

### objdump

- 反汇编目标文件或者可执行文件。

  ```shell
  -d 反汇编需要执行指令的那些section。
  -D 反汇编所有section。
  -h 显示Section Header信息。
  -x 显示全部Header信息。
  -s 除了显示全部Header信息，还显示对应的十六进制文件代码。
  ```

### hexdump

```hexdump```命令一般用来查看文件的十六进制编码，但实际上它能查看任何文件，而不只限于二进制文件。

```shell
#参数
-b: 单字节八进制显示
-c: 单字节字符显示
-C: 规范化 十六进制+ASCII显示
-d: 两字节十进制显示
-o: 两字节八进制显示
-x: 两字节十六进制显示
-e: 格式 用于显示数据的格式字符串
-f: 格式文件 包含格式字符串的文件
-n: 长度 只解释输入的指定长度个字节
-s: 偏移 跳过开头指定长度个字节
-v: 显示时不压缩相似的行
-V: 显示此帮助并退出

#常用
hexdump -c -s 0x1000 -n 100 a.out
```

## 进程

### top

- 按M，看内存占用。
- 按P，看CPU占用。
- 按组合键xb，然后用<>手动选择排序的列。
- `wa`代表`iowait`，是CPU等待IO完成操作花费的时间占CPU的百分比。

### iostat

- 显示实际硬盘读写情况。

### iotop

- 查看进程实际占用I/O。

### nohup

```nohup```(```no hang up```)，用于在系统后台不挂断地运行命令，退出终端不会影响程序的运行。在默认情况下，会输出一个名叫```nohup.out```的文件到当前目录下，如果当前目录的```nohup.out```文件不可写，输出重定向到```$HOME/nohup.out```文件中。

```shell 
#常用
nohup application [arg …] [2>&1] [&]
```

### ps

```Linux ps```(```process status```)命令用于显示当前进程的状态，类似于```windows```的任务管理器。

```shell
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

```pstree```命令以树状图显示进程间的关系(```display a tree of processes```)。```ps```命令可以显示当前正在运行的那些进程的信息，但是对于它们之间的关系却显示得不够清晰。在```Linux```系统中，系统调用```fork```可以创建子进程，通过```shell```也可以创建子进程，```Linux```系统中进程之间的关系天生就是一棵树，树的根就是进程```PID```为```1```的```init```进程。

```shell
#常用
pstree pid
pstree -p pid
```

### strace

```strace```常用来跟踪进程执行时的系统调用和所接收的信号。

```shell
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

- Linux性能计数器是一个基于内核的子系统，它提供一个性能分析框架，比如硬件(CPU、PMU(Performance Monitoring Unit))功能和软件(软件计数器、tracepoint)功能。
- 通过perf，应用程序可以利用PMU、tracepoint和内核中的计数器来进行性能统计。
- Perf可以对程序进行函数级别的采样，从而了解程序的性能瓶颈在哪里。其基本原理是：每隔一个固定时间，就是CPU上产生一个中断，看当前是哪个进程、哪个函数，然后给对应的进程和函数加一个统计值，这样就知道CPU有多少时间在某个进程或某个函数上了。

```shell
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

```shell
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

```shell
RX packets：接收的总包数
RX bytes：接收的字节数
RX errors：表示总的收包的错误数量
RX dropped：数据包已经进入了Ring Buffer，但是由于其它原因导致的丢包
RX overruns：表示了fifo的overruns，这是由于Ring Buffer不足导致的丢包
```
</details>

### tcpdump

![tcpdump](https://github.com/gongluck/images/blob/main/tcpdump/tcpdump.png)

```shell
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
