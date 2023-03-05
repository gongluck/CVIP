# TCP

- [TCP](#tcp)
  - [TCP 特性](#tcp-特性)
    - [主机到主机（Host-To-Host）](#主机到主机host-to-host)
    - [连接（Connection）](#连接connection)
    - [双工](#双工)
    - [可靠性](#可靠性)
  - [TCP 的握手和挥手](#tcp-的握手和挥手)
    - [TCP 协议的基本操作](#tcp-协议的基本操作)
    - [建立连接的过程（3 次握手）](#建立连接的过程3-次握手)
    - [断开连接的过程（4 次挥手）](#断开连接的过程4-次挥手)
    - [TCP 为什么是 3 次握手，4 次挥手？](#tcp-为什么是-3-次握手4-次挥手)
  - [Nagle 算法](#nagle-算法)
  - [TCP 的拆包和粘包](#tcp-的拆包和粘包)
  - [TCP Segment](#tcp-segment)
    - [Sequence Number 和 Acknowledgement Number](#sequence-number-和-acknowledgement-number)
    - [MSS（Maximun Segment Size）](#mssmaximun-segment-size)
  - [流速控制](#流速控制)
    - [请求/响应模型](#请求响应模型)
    - [滑动窗口（Sliding Window）](#滑动窗口sliding-window)
    - [重传](#重传)
    - [快速重传](#快速重传)
    - [流控](#流控)
  - [TIME\_WAIT](#time_wait)

## TCP 特性

### 主机到主机（Host-To-Host）

- TCP 提供的是 Host-To-Host 传输，一台主机通过 TCP 发送数据给另一台主机。
- TCP 协议往上是应用到应用（Application-To-Application）的协议。端口号用于区分应用。
- TCP 不负责实际地址到地址（Address-To-Address）的传输，因此 TCP 协议把 IP 地址给底层的互联网层处理。

### 连接（Connection）

- 连接是通信双方的一个约定，目标是让两个在通信的程序之间产生一个默契，保证两个程序都在线，而且尽快地响应对方的请求，这就是连接（Connection）。
- 连接是传输层的概念。
- 会话是应用的行为。连接会断开，聊天窗口没关，会话还在。

### 双工

- 任何时刻数据都可以双向收发，这就是全双工，全双工需要大于 1 条线路。
- 这里的线路，是一个抽象概念，可以并发地处理信号，达到模拟双工的目的。

### 可靠性

- 可靠性指数据保证无损传输。
- 如果发送方按照顺序发送，然后数据无序地在网络间传递，就必须有一种算法在接收方将数据恢复原有的顺序。

## TCP 的握手和挥手

- TCP 是一个连接导向的协议，设计有建立连接（握手）和断开连接（挥手）的过程。

### TCP 协议的基本操作

- 如果一个 Host 主动向另一个 Host 发起连接，称为 SYN（Synchronization），请求同步；
- 如果一个 Host 主动断开请求，称为 FIN（Finish），请求完成；
- 如果一个 Host 给另一个 Host 发送数据，称为 PSH（Push），数据推送。
- 以上 3 种情况，接收方收到数据后，都需要给发送方一个 ACK（Acknowledgement）响应。请求/响应的模型是可靠性的要求，如果一个请求没有响应，发送方可能会认为自己需要重发这个请求。

### 建立连接的过程（3 次握手）

- 因为要保持连接和可靠性约束，TCP 协议要保证每一条发出的数据必须给返回，返回数据叫作 ACK（也就是响应）。
  - 客户端发消息给服务端（SYN）_（1）_
  - 服务端准备好进行连接
  - 服务端针对客户端的 SYN 给一个 ACK _（2）_
  - 因为服务端还没有确定客户端是否准备好了。服务端马上给客户端发送数据，这个时候客户端可能还没有准备好接收数据。因此还需要增加下面过程
  - 服务端发送一个 SYN 给客户端 _（2）_
  - 客户端准备就绪
  - 客户端给服务端发送一个 ACK _（3）_

### 断开连接的过程（4 次挥手）

- 客户端要求断开连接，发送一个断开的请求，这个叫作（FIN） _（1）_
- 服务端收到请求，然后给客户端一个 ACK，作为 FIN 的响应 _（2）_
- 这个时候服务端不能马上传 FIN，因为断开连接要处理的问题比较多，比如说服务端可能还有发送出去的消息没有得到 ACK；也有可能服务端自己有资源要释放。因此断开连接不能像握手那样操作——将两条消息合并
- 服务端经过一个等待，确定可以关闭连接了，再发一条 FIN 给客户端 _（3）_
- 客户端收到服务端的 FIN，同时客户端也可能有自己的事情需要处理完，比如客户端有发送给服务端没有收到 ACK 的请求，客户端自己处理完成后，再给服务端发送一个 ACK _（4）_

### TCP 为什么是 3 次握手，4 次挥手？

- TCP 是一个双工协议，为了让双方都保证，建立连接的时候，连接双方都需要向对方发送 SYN（同步请求）和 ACK（响应）。
- 握手阶段双方都没有烦琐的工作，因此一方向另一方发起同步（SYN）之后，另一方可以将自己的 ACK 和 SYN 打包作为一条消息回复，因此是 3 次握手——需要 3 次数据传输。
- 到了挥手阶段，双方都可能有未完成的工作。收到挥手请求的一方，必须马上响应（ACK），表示接收到了挥手请求。
- 启用延迟确认的情况下是三次挥手。

## Nagle 算法

![Nagle算法](https://github.com/gongluck/images/blob/main/network/tcp/nagle.png)

- Nagle 算法默认是开启的，可以通过设置 socket 的 TCP_NODELAY 选项来关闭。

## TCP 的拆包和粘包

- TCP 是一个传输层协议。TCP 发送数据的时候，往往不会将数据一次性发送，而是将数据拆分成很多个部分，然后再逐个发送。
- 为了稳定性，一次发送的数据越多，出错的概率越大。
- 为了效率，网络中有时候存在着并行的路径，拆分数据包就能更好地利用这些并行的路径。
- 发送和接收数据的时候，都存在着缓冲区。大量的应用频繁地通过网卡收发数据，这个时候，网卡只能一个一个处理应用的请求。当网卡忙不过来的时候，数据就需要排队，也就是将数据放入缓冲区。
- TCP 拆包的作用是将任务拆分处理，降低整体任务出错的概率，以及减小底层网络处理的压力。拆包过程需要保证数据经过网络的传输，又能恢复到原始的顺序。TCP 利用（发送字节数、接收字节数）的唯一性来确定封包之间的顺序关系。
- 粘包是为了防止数据量过小，导致大量的传输，而将多个 TCP 段合并成一个发送。

## TCP Segment

![TCP段](https://github.com/gongluck/images/blob/main/network/tcp/tcp_segment.png)

- **Source Port/Destination Port** 描述的是发送端口号和目标端口号，代表发送数据的应用程序和接收数据的应用程序。
- **Sequence Number 和 Acknowledgment Number** 是保证可靠性的两个关键。
- Data Offset 是一个偏移量。这个量存在的原因是 TCP Header 部分的长度是可变的，因此需要一个数值来描述数据从哪个字节开始。
- URG/ACK/PSH/RST/SYN/FIN 是几个标志位，用于描述 TCP 段的行为。
  - URG 代表这是一个紧急数据，比如远程操作的时候，用户按下了 Ctrl+C，要求终止程序，这种请求需要紧急处理。
  - **ACK 代表响应**，所有的消息都必须有 ACK，这是 TCP 协议确保稳定性的一环。
  - **PSH 代表数据推送**，也就是在传输数据的意思。
  - **SYN 同步请求**，也就是申请握手。
  - **FIN 终止请求**，也就是挥手。
- **Window 也是 TCP 保证稳定性并进行流量控制的工具**。
- Checksum 是校验和，用于校验 TCP 段有没有损坏。
- Urgent Pointer 指向最后一个紧急数据的序号（Sequence Number）。有时候紧急数据是连续的很多个段，所以需要提前告诉接收方进行准备。
- Options 中存储了一些可选字段，比如 MSS（Maximun Segment Size）。
- Padding 存在的意义是因为 Options 的长度不固定，需要 Pading 进行对齐。

### Sequence Number 和 Acknowledgement Number

- 发送数据的时候，为每一个 TCP 段分配一个自增的 Sequence Number。接收数据的时候，虽然得到的是乱序的 TCP 段，但是可以通过 Seq 进行排序。
- 每个 TCP 段发送时，发送方已经接收了多少数据，用 Acknowledgement Number 表示。

### MSS（Maximun Segment Size）

- MSS 是 TCP Header 中的可选项（Options），这个可选项控制了 TCP 段的大小，它是一个协商字段（Negotiate）。协议是双方都要遵循的标准，因此配置往往不能由单方决定，需要双方协商。
- TCP 段的大小（MSS）涉及发送、接收缓冲区的大小设置，双方实际发送接收封包的大小，对拆包和粘包的过程有指导作用，因此需要双方去协商。
- 如果这个字段设置得非常大，对方可能会拒绝，作为服务的提供方，可能不会愿意接收太大的 TCP 段。因为大的 TCP 段，会降低性能，比如内存使用的性能。还有就是资源的占用。一个用户占用服务器太多的资源，意味着其他的用户就需要等待或者降低他们的服务质量。支持 TCP 协议工作的 IP 协议，工作效率会下降。TCP 协议不肯拆包，IP 协议就需要拆出大量的包。因为在网络中，每次能够传输的数据不可能太大，这受限于具体的网络传输设备，也就是物理特性。但是 IP 协议，拆分太多的封包并没有意义。因为可能会导致属于同个 TCP 段的封包被不同的网络路线传输，这会加大延迟。同时，拆包，还需要消耗硬件和计算资源。
- MSS 太小的情况下，会浪费传输资源（降低吞吐量）。因为数据被拆分之后，每一份数据都要增加一个头部。如果 MSS 太小，那头部的数据占比会上升，这让吞吐量成为一个灾难。

## 流速控制

### 请求/响应模型

- TCP 中每个发送的请求都需要响应。如果一个请求没有收到响应，发送方就会认为这次发送出现了故障，会触发重发。
- 让发送方有请求就发送出去，而不是等待响应。通过这样的处理方式，发送的数据连在了一起，响应的数据也连在了一起，吞吐量就提升了。

### 滑动窗口（Sliding Window）

![滑动窗口](https://github.com/gongluck/images/blob/main/network/tcp/sliding_window.png)

### 重传

- 如果发送过程中，部分数据没能收到 ACK 就可能发生重传。
- 如果后来段重传成功（接收到 ACK），那么窗口就会继续右移。如果段发送失败，还是没能收到 ACK，那么接收方也会抛弃后面已接收段。这样从失败段开始之后的数据都需要重发。

### 快速重传

- 在 TCP 协议中，如果接收方想丢弃某个段，可以选择不发 ACK。发送端超时后，会重发这个 TCP 段。而有时候，接收方希望催促发送方尽快补发某个 TCP 段，这个时候可以使用快速重传能力。
- 例如段 1、段 2、段 4 到了，但是段 3 没有到。 接收方可以发送多次段 3 的 ACK。如果发送方收到多个段 3 的 ACK，就会重发段 3。这个机制称为快速重传。这和超时重发不同，是一种催促的机制。
- 为了不让发送方误以为段 3 已经收到了，在快速重传的情况下，接收方即便收到发来的段 4，依然会发段 3 的 ACK（不发段 4 的 ACK），直到发送方把段 3 重传。
- 实际操作中，每个 TCP 段的大小不同，限制数量会让接收方的缓冲区不好操作，因此实际操作中窗口大小单位是字节数。

### 流控

- 发送、接收窗口的大小可以用来控制 TCP 协议的流速。
- 窗口越大，同时可以发送、接收的数据就越多，支持的吞吐量也就越大。
- 窗口越大，如果数据发生错误，损失也就越大，因为需要重传越多的数据。
- 在任何一个时刻，TCP 发送缓冲区的数据是否能真正发送出去，至少取决于两个因素，一个是当前的发送窗口大小，另一个是拥塞窗口大小，而 TCP 协议中总是取两者中最小值作为判断依据。

## TIME_WAIT

![四次挥手过程](https://github.com/gongluck/images/blob/main/network/tcp/close.png)

- 只有发起连接终止的一方会进入 TIME_WAIT 状态。
- ACK n + 1 丢失时，如果主机 1 没有维护 TIME_WAIT 状态，而直接进入 CLOSED 状态，它就失去了当前状态的上下文，只能回复一个 RST 操作，从而导致被动关闭方出现错误。
- 现在主机 1 知道自己处于 TIME_WAIT 的状态，ACK n + 1 丢失时，就可以在接收到 FIN 报文之后，重新发出一个 ACK 报文，使得主机 2 可以进入正常的 CLOSED 状态。
- 经过 2MSL 这个时间，足以让两个方向上的分组都被丢弃，使得原来连接的分组在网络中都自然消失，再出现的分组一定都是新化身所产生的。
- `net.ipv4.tcp_tw_reuse` 如果是安全可控（客户端并且 TIME_WAIT 状态超过 1 s）的，可以复用处于 TIME_WAIT 的套接字为新的连接所用。使用这个选项，需要打开对 TCP 时间戳的支持，即 `net.ipv4.tcp_timestamps = 1`（默认即为 1）。
- `SO_REUSEADDR` 套接字选项，如果端口已被占用，但是 TCP 连接状态位于 TIME_WAIT ，可以重用端口。如果端口忙，而 TCP 处于其他状态，重用端口时依旧得到"Address already in use"的错误信息。