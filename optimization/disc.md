# 磁盘

- [磁盘](#磁盘)
  - [零拷贝](#零拷贝)
  - [PageCache](#pagecache)
  - [异步IO](#异步io)


## 零拷贝

![文件网络发送](https://github.com/gongluck/images/blob/main/disc/文件网络发送.png)

- 上下文切换的成本并不小，虽然一次切换仅消耗几十纳秒到几微秒，但高并发服务会放大这类时间的消耗。
- 过多的内存拷贝无谓地消耗了CPU资源，降低了系统的并发处理能力。

![零拷贝](https://github.com/gongluck/images/blob/main/disc/零拷贝.png)
![网卡SG-DMA](https://github.com/gongluck/images/blob/main/disc/网卡SG-DMA.png)

- 零拷贝是操作系统提供的新函数，同时接收文件描述符和TCP socket作为输入参数，这样执行时就可以完全在内核态完成内存拷贝，既减少了内存拷贝次数，也降低了上下文切换次数。

## PageCache

- 读取文件时，是先把磁盘文件拷贝到PageCache上，再拷贝到进程中。
- 刚被访问的数据在短时间内再次被访问的概率很高(时间局部性原理)，用PageCache缓存最近访问的数据，当空间不足时淘汰最久未被访问的缓存(LRU算法)。
- 读磁盘时优先到PageCache中找一找，如果数据存在便直接返回，这便大大提升了读磁盘的性能。
- PageCache使用了`预读`功能。内核会把其目标后的一部分数据也读取到PageCache，这后读取的成本很低。
- 在某些情况下，PageCache会不起作用([缓存未命中](./cpu.md#cpu多级缓存))，甚至由于多做了一次内存拷贝，造成性能的降低。
- 高并发场景下，为了防止PageCache被大文件占满后不再对小文件产生作用，大文件不应使用PageCache，进而也不应使用零拷贝技术处理。

## 异步IO

![异步IO](https://github.com/gongluck/images/blob/main/disc/异步IO.png)

  - 异步IO并没有拷贝到PageCache中，这其实是异步IO实现上的缺陷。
  - 经过PageCache的IO称为缓存IO，与虚拟内存系统耦合太紧，导致异步IO从诞生起到现在都不支持缓存IO。
  - 绕过PageCache的IO称为直接IO。对于磁盘，异步IO只支持直接IO。
