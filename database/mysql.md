# MySQL

- [MySQL](#mysql)
  - [MySQL 执行流程](#mysql-执行流程)
  - [主从备份](#主从备份)
  - [主从切换](#主从切换)
  - [主从备份](#主从备份-1)
  - [redo log 和 binlog](#redo-log-和-binlog)
  - [隔离级别](#隔离级别)
    - [可重复读](#可重复读)
    - [读提交](#读提交)
  - [索引](#索引)
  - [事务](#事务)
  - [锁](#锁)
  - [](#)
  - [排序](#排序)
  - [幻读](#幻读)
  - [性能监控](#性能监控)

## MySQL 执行流程

![MySQL执行流程](https://github.com/gongluck/images/blob/main/database/mysql/mysql.png)

## 主从备份

![主从备份](https://github.com/gongluck/images/blob/main/database/mysql/master_slave.png)

## 主从切换

- 可靠性优先策略

  ![可靠性优先策略](https://github.com/gongluck/images/blob/main/database/mysql/master_slave_change1.png)

  - 在满足数据可靠性的前提下，MySQL 高可用系统的可用性，是依赖于主备延迟的。延迟的时间越小，在主库故障的时候，服务恢复需要的时间就越短，

- 可用性优先策略

![可用性优先策略](https://github.com/gongluck/images/blob/main/database/mysql/master_slave_change2.png)

## 主从备份

- 在基于 GTID 的主备关系里，系统认为只要建立主备关系，就必须保证主库发给备库的日志是完整的。因此，如果实例 B 需要的日志已经不存在，A’就拒绝把日志发给 B。
- 延迟复制的备库是一种特殊的备库，通过 CHANGE MASTER TO MASTER_DELAY = N 命令，可以指定这个备库持续保持跟主库有 N 秒的延迟。

## redo log 和 binlog

- redo log 是 InnoDB 引擎特有的；binlog 是 MySQL 的 Server 层实现的，所有引擎都可以使用。
- redo log 是物理日志，记录的是“在某个数据页上做了什么修改”；binlog 是逻辑日志，记录的是这个语句的原始逻辑，比如“给 ID=2 这一行的 c 字段加 1 ”。
- redo log 是循环写的，空间固定会用完；binlog 是可以追加写入的。“追加写”是指 binlog 文件写到一定大小后会切换到下一个，并不会覆盖以前的日志。
- 只要 redo log 和 binlog 保证持久化到磁盘，就能确保 MySQL 异常重启后，数据可以恢复。
- 事务执行过程中，先把日志写到 binlog cache，事务提交的时候，再把 binlog cache 写到 binlog 文件中。
- 事务在执行过程中，生成的 redo log 是要先写到 redo log buffer 的。
- InnoDB 有一个后台线程，每隔 1 秒，就会把 redo log buffer 中的日志，调用 write 写到文件系统的 page cache，然后调用 fsync 持久化到磁盘。
- 两阶段提交的时候说过，时序上 redo log 先 prepare， 再写 binlog，最后再把 redo log commit。
- 组提交机制，可以大幅度降低磁盘的 IOPS 消耗。
- binlog_format=statement 时，binlog 里面记录的就是 SQL 语句的原文。
- 当 binlog_format 使用 row 格式的时候，binlog 里面记录了真实操作行的主键 id。
- mixed 格式的意思是，MySQL 自己会判断这条 SQL 语句是否可能引起主备不一致，如果有可能，就用 row 格式，否则就用 statement 格式。

## 隔离级别

### 可重复读

- 可重复读隔离级别遵守两阶段锁协议，所有加锁的资源，都是在事务提交或者回滚的时候才释放的。

### 读提交

- 语句执行过程中加上的行锁，在语句执行完成后，就要把“不满足条件的行”上的行锁直接释放了，不需要等到事务提交。

## 索引

- 在 InnoDB 中，表都是根据主键顺序以索引的形式存放的，这种存储方式的表称为索引组织表。又因为 InnoDB 使用了 B+ 树索引模型，所以数据都是存储在 B+ 树中的。
- 索引类型分为主键索引和非主键索引。主键索引的叶子节点存的是整行数据。在 InnoDB 里，主键索引也被称为聚簇索引（clustered index）。非主键索引的叶子节点内容是主键的值。在 InnoDB 里，非主键索引也被称为二级索引（secondary index）。基于非主键索引的查询需要多扫描一棵索引树。回到主键索引树搜索的过程称为回表。
- 自增主键的插入数据模式，每次插入一条新记录，都是追加操作，都不涉及到挪动其他记录，也不会触发叶子节点的分裂。主键长度越小，普通索引的叶子节点就越小，普通索引占用的空间也就越小。从性能和存储空间方面考量，自增主键往往是更合理的选择。
- 不只是索引的全部定义，只要满足最左前缀，就可以利用索引来加速检索。这个最左前缀可以是联合索引的最左 N 个字段，也可以是字符串索引的最左 M 个字符。如果通过调整顺序，可以少维护一个索引，那么这个顺序往往就是需要优先考虑采用的。MySQL 5.6 引入的索引下推优化（index condition pushdown）， 可以在索引遍历过程中，对索引中包含的字段先做判断，直接过滤掉不满足条件的记录，减少回表次数。
- 覆盖索引是指，索引上的信息足够满足查询请求，不需要再回到主键索引上去取数据。
- 对索引字段做函数操作，可能会破坏索引值的有序性，因此优化器就决定放弃走树搜索功能。
- InnoDB 的数据是保存在主键索引上的。
- MySQL 是“边读边发的”，这个概念很重要。这就意味着，如果客户端接收得慢，会导致 MySQL 服务端由于结果发不出去，这个事务的执行时间变长。
- 对于 InnoDB 引擎内部，由于有淘汰策略，大查询也不会导致内存暴涨。并且，由于 InnoDB 对 LRU 算法做了改进，冷数据的全表扫描，对 Buffer Pool 的影响也能做到可控。
- 回表是指，InnoDB 在普通索引 a 上查到主键 id 的值后，再根据一个个主键 id 的值到主键索引上去查整行数据的过程。
- 如果按照主键的递增顺序查询的话，对磁盘的读比较接近顺序读，能够提升读性能。
- InnoDB 引擎把数据放在主键索引上，其他索引上保存的是主键 id。这种方式，我们称之为索引组织表（Index Organizied Table）。
  而 Memory 引擎采用的是把数据单独存放，索引上保存数据位置的数据组织形式，我们称之为堆组织表（Heap Organizied Table）。
  insert into … on duplicate key update 这个语义的逻辑是，插入一行数据，如果碰到唯一键约束，就执行后面的更新语句。

## 事务

- GTID 的全称是 Global Transaction Identifier，也就是全局事务 ID，是一个事务在提交的时候生成的，是这个事务的唯一标识。

## 锁

- MySQL 里面的锁大致可以分成全局锁、表级锁和行锁三类。
- MySQL 提供了一个加全局读锁的方法，命令是 `Flush tables with read lock` (FTWRL)。当需要让整个库处于只读状态的时候，可以使用这个命令，之后其他线程的以下语句会被阻塞：数据更新语句（数据的增删改）、数据定义语句（包括建表、修改表结构等）和更新类事务的提交语句。
- 官方自带的逻辑备份工具是 mysqldump。当 mysqldump 使用参数 `–single-transaction` 的时候，导数据之前就会启动一个事务，来确保拿到一致性视图。而由于 MVCC 的支持，这个过程中数据是可以正常更新的。single-transaction 方法只适用于所有的表使用事务引擎的库。如果有的表使用了不支持事务的引擎，那么备份就只能通过 FTWRL 方法。
- MySQL 里面表级别的锁有两种：一种是表锁，一种是元数据锁（meta data lock，MDL）。
- 表锁的语法是 `lock tables … read/write`。与 FTWRL 类似，可以用 `unlock tables` 主动释放锁，也可以在客户端断开的时候自动释放。lock tables 语法除了会限制别的线程的读写外，也限定了本线程接下来的操作对象。在 MySQL 5.5 版本中引入了 MDL，当对一个表做增删改查操作的时候，加 MDL 读锁；当要对表做结构变更操作的时候，加 MDL 写锁。MyISAM 引擎就不支持行锁。
- 不支持行锁意味着并发控制只能使用表锁，对于这种引擎的表，同一张表上任何时刻只能有一个更新在执行，这就会影响到业务并发度。InnoDB 是支持行锁的，这也是 MyISAM 被 InnoDB 替代的重要原因之一。在 InnoDB 事务中，行锁是在需要的时候才加上的，但并不是不需要了就立刻释放，而是要等到事务结束时才释放。这个就是两阶段锁协议。如果事务中需要锁多个行，要把最可能造成锁冲突、最可能影响并发度的锁尽量往后放。
- InnoDB 的默认事务隔离级别是可重复读。
- 锁是加在索引上的；同时，它给我们的指导是，如果你要用 lock in share mode 来给行加读锁避免数据被更新的话，就必须得绕过覆盖索引的优化，在查询字段中加入索引中不存在的字段。
- 在删除数据的时候尽量加 limit。这样不仅可以控制删除数据的条数，让操作更安全，还可以减小加锁的范围。
- 分析加锁规则的时候可以用 next-key lock 来分析。具体执行的时候，是要分成间隙锁和行锁两段来执行的。
- next-key lock 实际上是由间隙锁加行锁实现的。如果切换到读提交隔离级别 (read-committed) 的话，就好理解了，过程中去掉间隙锁的部分，也就是只剩下行锁的部分。

##

- MySQL 会根据词法解析的结果分析出可能可以使用的索引作为候选项，然后在候选列表中依次判断每个索引需要扫描多少行。如果 force index 指定的索引在候选索引列表中，就直接选择这个索引，不再评估其他索引的执行代价。
- delete 命令其实只是把记录的位置，或者数据页标记为了“可复用”，但磁盘文件的大小是不会变的。也就是说，通过 delete 命令是不能回收表空间的。
- InnoDB 是索引组织表，主键索引树的叶子节点是数据，而普通索引树的叶子节点是主键值。所以，普通索引树比主键索引树小很多。

## 排序

- 如果 MySQL 实在是担心排序内存太小，会影响排序效率，才会采用 rowid 排序算法，这样排序过程中一次可以排序更多行，但是需要再回到原表去取数据。
- 如果 MySQL 认为内存足够大，会优先选择全字段排序，把需要的字段都放到 sort_buffer 中，这样排序后就会直接从内存里面返回查询结果了，不用再回到原表去取数据。
- order by rand() 使用了内存临时表，内存临时表排序的时候使用了 rowid 排序方法。

## 幻读

- 幻读指的是一个事务在前后两次查询同一个范围的时候，后一次查询看到了前一次查询没有看到的行。幻读仅专指“新插入的行”。
- 在可重复读隔离级别下，普通的查询是快照读，是不会看到别的事务插入的数据的。因此，幻读在“当前读”（for update）下才会出现。当前读的规则，就是要能读到所有已经提交的记录的最新值。
- 产生幻读的原因是，行锁只能锁住行，但是新插入记录这个动作，要更新的是记录之间的“间隙”。因此，为了解决幻读问题，InnoDB 只好引入新的锁，也就是间隙锁 (Gap Lock)。
- 执行 select \* from t where k=v for update 的时候，在一行行扫描的过程中，不仅将给行加上了行锁，还给行两边的空隙，也加上了间隙锁。这样就确保了无法再插入新的记录。
- 间隙锁的引入，可能会导致同样的语句锁住更大的范围，这其实是影响了并发度的。
- 间隙锁是在可重复读隔离级别下才会生效的。间隙锁的引入会影响系统的并发度，也增加了锁分析的复杂度。

## 性能监控

- MySQL 5.6 版本以后提供的 performance_schema 库，就在 file_summary_by_event_name 表里统计了每次 IO 请求的时间。
- 每一次操作数据库，performance_schema 都需要额外地统计这些信息，所以我们打开这个统计功能是有性能损耗的。
