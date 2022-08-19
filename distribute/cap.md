# CAP

- [CAP](#cap)
  - [CAP理论](#cap理论)
  - [BASE理论](#base理论)
  - [舍弃一致性提升性能](#舍弃一致性提升性能)

## CAP理论

- 当数据同时存放在多个主机上时，可用性与一致性是不可兼得的。
- 根据CAP的指导性思想，可以通过牺牲一致性，来提升可用性中的核心因素：性能。
- CAP包括分布式系统最核心的3个特性

  ![一致性](https://github.com/gongluck/images/blob/main/distribute/一致性.png)

  - Consistency 一致性

  ![可用性](https://github.com/gongluck/images/blob/main/distribute/可用性.png)

  - Availability 可用性
  - Partition tolerance 分区容错性

## BASE理论

- 实际工程中一致性与可用性的边界会模糊很多，因此又有了最终一致性这样一个概念，这个"最终"究竟是多久，将由业务特性、网络故障率等因素综合决定。
- 伴随最终一致性的是BASE理论
  - Basically Available 基本可用性
  - Soft state 软状态
  - Eventually consistent 最终一致性

## 舍弃一致性提升性能

- 在纵向上在离客户端更近的位置增加数据的副本，并把它存放在处理速度更快的物理介质上，就可以作为缓存降低请求的时延。
- 在横向上对数据增加副本，并在这些主机间同步数据，这样工作在数据副本上的进程也可以同时对客户端提供服务，这就增加了系统的并发性。
- 有很多以写操作触发缓存更新的设计，通常又分为write back和write through两种模式
  - write back 牺牲了更多的一致性，但带来了更低的请求时延。
  - write through 会在更新数据成功后再更新缓存，虽然带来了很好的一致性，但写操作的时延会更久。
