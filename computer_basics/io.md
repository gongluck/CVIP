# IO

- [IO](#io)
  - [索引节点和目录项](#索引节点和目录项)
  - [文件系统](#文件系统)
  - [通用块层](#通用块层)
  - [RAID](#raid)
  - [IO 栈](#io-栈)
  - [性能分析](#性能分析)
  - [性能工具](#性能工具)
    - [传统工具](#传统工具)
    - [BPF工具](#bpf工具)

## 索引节点和目录项

- Linux 文件系统为每个文件都分配两个数据结构，`索引节点（index node）`和`目录项（directory entry）`。它们主要用来记录文件的元信息和目录结构。
- 索引节点，简称为 inode，用来记录文件的元数据，比如 inode 编号、文件大小、访问权限、修改日期、数据的位置等。索引节点和文件一一对应，它跟文件内容一样，都会被持久化存储到磁盘中。所以引节点同样占用磁盘空间。
- 目录项，简称为 dentry，用来记录文件的名字、索引节点指针以及与其他目录项的关联关系。多个关联的目录项，就构成了文件系统的目录结构。不过，不同于索引节点，目录项是由内核维护的一个内存数据结构，所以通常也被叫做目录项缓存。
- 索引节点是每个文件的唯一标志，而目录项维护的正是文件系统的树状结构。目录项和索引节点的关系是多对一。
- 索引节点的容量，（也就是 Inode 个数）是在格式化磁盘时设定好的，一般由格式化工具自动生成。
- 目录项本身就是一个内存缓存，而索引节点则是存储在磁盘中的数据。索引节点自然也会缓存到内存中，加速文件的访问。

## 文件系统

- 为了支持各种不同的文件系统，Linux 内核在用户进程和文件系统的中间，又引入了一个抽象层，也就是`虚拟文件系统 VFS（Virtual File System）`。
- VFS 为上层的应用程序，提供标准的文件访问接口；对下会通过通用块层，来存储和管理磁盘数据。

## 通用块层

- 为了减小不同块设备的差异带来的影响，Linux 通过一个统一的通用块层，来管理各种不同的块设备。
- 通用块层还会给文件系统和应用程序发来的 I/O 请求排队，并通过重新排序、请求合并等方式，提高磁盘读写的效率。

## RAID

- 把多块磁盘组合成一个逻辑磁盘，构成冗余独立磁盘阵列，也就是 RAID（Redundant Array of Independent Disks），从而可以提高数据访问的性能，并且增强数据存储的可靠性。

## IO 栈

![IO栈](https://github.com/gongluck/images/blob/main/io/io_stack.png)
![数据状态](https://github.com/gongluck/images/blob/main/io/data_status.png)

## 性能分析

![IO性能分析](https://github.com/gongluck/images/blob/main/linux/performance/io.png)

- 在[top](../tools/command.md#top)命令的输出结果里面，有一行是以 %CPU 开头的这一行里，有一个叫作`wa`的指标，这个指标就代表着`iowait`，也就是 CPU 等待 IO 完成操作花费的时间占 CPU 的百分比。
- [iostat](../tools/command.md#iostat)命令能够看到实际的硬盘读写情况。
- [iotop](../tools/command.md#iotop)命令，可以看到具体是哪一个进程实际占用了大量 I/O。
- 可以`用追加写代替随机写`，减少寻址开销，加快 I/O 写的速度。
- 可以借助`缓存 I/O `，充分利用系统缓存，降低实际 I/O 的次数。
- 可以在应用程序内部构建自己的`缓存`，或者用 Redis 这类外部缓存系统。这样，一方面，能在应用程序内部，控制缓存的数据和生命周期；另一方面，也能降低其他应用程序使用缓存对自身的影响。
- 在需要频繁读写同一块磁盘空间时，可以用 `mmap` 代替 read/write，减少内存的拷贝次数。
- 在需要同步写的场景中，尽量将`写请求合并`，而不是让每个请求都同步写入磁盘，即可以用 `fsync()` 取代 O_SYNC。
- 在多个应用程序共享相同磁盘时，为了保证 I/O 不被某个应用完全占用，推荐你使用 `cgroups` 的 I/O 子系统，来限制进程/进程组的 IOPS 以及吞吐量。
- 在使用 CFQ 调度器时，可以用 ionice 来调整进程的 `I/O 调度优先级`，特别是提高核心应用的 I/O 优先级。ionice 支持三个优先级类：Idle、Best-effort 和 Realtime。其中， Best-effort 和 Realtime 还分别支持 0-7 的级别，数值越小，则表示优先级别越高。

## 性能工具

### 传统工具

- [df](../tools/command.md#df)
- [mount](../tools/command.md#mount)
- [strace](../tools/command.md#strace)
- [perf](../tools/command.md#perf)
- [iostat](../tools/command.md#iostat)

### BPF工具

- opensnoop[-bpfcc/.bt]
- statsnoop[-bpfcc/.bt]
- syncsnoop[-bpfcc/.bt]
- vfsstat[-bpfcc/.bt]
- vfscount[-bpfcc/.bt]
- fileslower[-bpfcc]
- filetop[-bpfcc]
- cachestat[-bfpcc]
- dcstat[-bpfcc]
- dcsnoop[-bpfcc/.bt]
- mountsnoop[-bpfcc]
- biolatency[-bpfcc/.bt]
- biosnoop[-bpfcc/.bt]
- biotop[-bpfcc]
- bitsize[-bpfcc/.bt]
- mdflush[-bpfcc/.bt]