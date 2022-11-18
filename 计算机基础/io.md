# IO

- [IO](#io)
  - [IO性能分析](#io性能分析)

## IO性能分析

- 在[top](../tools/shell.md#top)命令的输出结果里面，有一行是以%CPU开头的这一行里，有一个叫作`wa`的指标，这个指标就代表着`iowait`，也就是CPU等待IO完成操作花费的时间占CPU的百分比。
- [iostat](../tools/shell.md#iostat)命令能够看到实际的硬盘读写情况。
- [iotop](../tools/shell.md#iotop)命令，可以看到具体是哪一个进程实际占用了大量I/O。
