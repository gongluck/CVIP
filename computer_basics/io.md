# IO

- [IO](#io)
  - [数据落盘过程](#数据落盘过程)
  - [性能分析](#性能分析)

## 数据落盘过程

![数据状态](https://github.com/gongluck/images/blob/main/io/data_status.png)

## 性能分析

![IO性能分析](https://github.com/gongluck/images/blob/main/linux/performance/io.png)

- 在[top](../tools/command.md#top)命令的输出结果里面，有一行是以 %CPU 开头的这一行里，有一个叫作`wa`的指标，这个指标就代表着`iowait`，也就是 CPU 等待 IO 完成操作花费的时间占 CPU 的百分比。
- [iostat](../tools/command.md#iostat)命令能够看到实际的硬盘读写情况。
- [iotop](../tools/command.md#iotop)命令，可以看到具体是哪一个进程实际占用了大量 I/O。
- 可以用追加写代替随机写，减少寻址开销，加快 I/O 写的速度。
- 可以借助缓存 I/O ，充分利用系统缓存，降低实际 I/O 的次数。
- 可以在应用程序内部构建自己的缓存，或者用 Redis 这类外部缓存系统。这样，一方面，能在应用程序内部，控制缓存的数据和生命周期；另一方面，也能降低其他应用程序使用缓存对自身的影响。
- 在需要频繁读写同一块磁盘空间时，可以用 mmap 代替 read/write，减少内存的拷贝次数。
- 在需要同步写的场景中，尽量将写请求合并，而不是让每个请求都同步写入磁盘，即可以用 fsync() 取代 O_SYNC。
- 在多个应用程序共享相同磁盘时，为了保证 I/O 不被某个应用完全占用，推荐你使用 cgroups 的 I/O 子系统，来限制进程/进程组的 IOPS 以及吞吐量。
- 在使用 CFQ 调度器时，可以用 ionice 来调整进程的 I/O 调度优先级，特别是提高核心应用的 I/O 优先级。ionice 支持三个优先级类：Idle、Best-effort 和 Realtime。其中， Best-effort 和 Realtime 还分别支持 0-7 的级别，数值越小，则表示优先级别越高。
