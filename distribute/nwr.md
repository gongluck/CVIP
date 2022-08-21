# NWR

- [NWR](#nwr)
  - [NWR算法](#nwr算法)
  - [Last Write Win](#last-write-win)

## NWR算法

- NWR算法提供了一个很棒的读写模型，可以解决去中心化下的同步问题。
- 在去中心化系统中将1份数据存放在N个节点上，每次操作时，写W个节点、读R个节点，只要调整W、R与N的关系，就能动态地平衡一致性与性能。
- 如果冗余数据存放在N个节点上，且每次写操作成功写入W个节点，其他N-W个节点将异步地同步数据，而读操作则从R个节点中选择并读出正确的数据，只要确保W+R>N，同1条数据的读、写操作就不能并发执行，这样客户端就总能读到最新写入的数据。特别是当W>N/2时，同1条数据的修改必然是顺序执行的。这样，分布式系统就具备了强一致性，这也是NWR算法的由来。
- 只要读、写操作涉及的节点超过半数，就注定读写操作总包含一个含有正确数据的节点。

## Last Write Win

  ![LastWriteWin](https://github.com/gongluck/images/blob/main/distribute/LastWriteWin.png)