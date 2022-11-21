# 内存

- [内存](#内存)
  - [内存池](#内存池)
    - [Ptmalloc2](#ptmalloc2)
    - [tcmalloc](#tcmalloc)
  - [栈](#栈)


## 内存池

![内存池](https://github.com/gongluck/images/blob/main/memory/内存池.png)

- [C库内存池](../stl/allocator.md#内存分配和释放)工作时，会预分配比你申请的字节数更大的空间作为内存池。

### Ptmalloc2

- Ptmalloc2对多线程下小内存的分配特别友好，允许内存池在线程间复用以提升性能。
- 每次分配内存，Ptmalloc2一定要加锁，才能解决共享资源的互斥问题。
- 线程数越多，Ptmalloc2出现锁竞争的概率就越高。
- Ptmalloc2更擅长大内存的分配。

### tcmalloc

- tcmalloc是Google开源的一个内存管理库，作为glibc malloc的替代品。目前已经在chrome、safari等知名软件中运用。
- tcmalloc为每个线程分配了一个线程本地ThreadCache，小内存从ThreadCache分配，此外还有个中央堆(CentralCache)，ThreadCache不够用的时候，会从CentralCache中获取空间放到ThreadCache中。
- TCMalloc对中等内存、大内存的分配速度很慢。

## 栈

- 每个线程都有独立的栈，所以分配内存时不需要加锁保护，而且栈上对象的尺寸在编译阶段就已经写入可执行文件了，执行效率更高！
- 栈内存生命周期有限，会随着函数调用结束后自动释放。
- 栈的容量有限，如果申请的内存超过限制会造成栈溢出错误。
